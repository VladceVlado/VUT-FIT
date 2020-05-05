package dbs.mind_maps.models;

import oracle.jdbc.internal.OraclePreparedStatement;
import oracle.jdbc.internal.OracleResultSet;
import oracle.ord.im.OrdImage;
import oracle.spatial.geometry.JGeometry;
import oracle.sql.STRUCT;

import javax.imageio.ImageIO;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;

import java.awt.*;
import java.awt.geom.Area;
import java.awt.geom.Point2D;
import java.awt.geom.Rectangle2D;

import java.sql.Statement;
import java.util.*;
import java.util.List;

import static java.awt.geom.PathIterator.SEG_CLOSE;
import static java.awt.geom.PathIterator.SEG_CUBICTO;


public class Node
{
    private static final String SQL_CAN_UPDATE = "SELECT node_move(?, ?) FROM DUAL";
    private Integer ID = null;
    public Integer diagramID;

    public String title;
    public String text;
    public Color color;
    public Paint borderColor = Color.BLACK;
    public Shape shape;
    private BufferedImage img = null;
    private boolean imgUpdated = false;

    public Boolean selected = false;
    public Boolean resized = false;

    public static void deleteNode(Node node) throws DatabaseHandler.LoginException, SQLException
    {
        final String QUERY = "DELETE FROM nodes WHERE id = " + node.ID;
        PreparedStatement statement = DatabaseHandler.getInstance().getConnection().prepareStatement(QUERY);
        statement.executeQuery();
    }

    public static void deleteImageFromDb(Node node) throws DatabaseHandler.LoginException, SQLException
    {
        final String QUERY3 = "UPDATE nodes SET image_ac = null WHERE id = " + node.ID;
        final String QUERY2 = "UPDATE nodes SET image_si = null WHERE id = " + node.ID;
        final String QUERY1 = "UPDATE nodes SET image = null WHERE id = " + node.ID;


        PreparedStatement statement3 = DatabaseHandler.getInstance().getConnection().prepareStatement(QUERY3);
        PreparedStatement statement2 = DatabaseHandler.getInstance().getConnection().prepareStatement(QUERY2);
        PreparedStatement statement1 = DatabaseHandler.getInstance().getConnection().prepareStatement(QUERY1);

        statement3.executeQuery();
        statement2.executeQuery();
        statement1.executeQuery();
    }

    final static private String SQL_GET_AREA = "SELECT SDO_GEOM.SDO_AREA(shape,0.005) FROM nodes WHERE " +
            "SDO_EQUAL(SDO_GEOM.SDO_INTERSECTION(shape, ?, 0.005), shape) = 'TRUE'";
    public static double getSelectedArea(Rectangle selection)
        throws SQLException, JGeometryToShapeException, DatabaseHandler.LoginException
    {
        PreparedStatement pstmt = DatabaseHandler.getInstance().getConnection().prepareStatement(SQL_GET_AREA);
        STRUCT obj = JGeometry.store(shapeToJGeometry(selection), DatabaseHandler.getInstance().getConnection());
        pstmt.setObject(1, obj);
        var resultSet = pstmt.executeQuery();

        double sum = 0;
        while (resultSet.next()) {
            sum += resultSet.getDouble(1);
        }

        return sum;
    }

    final static private String SQL_GET_MIN_DISTANCE = "SELECT SDO_GEOM.SDO_DISTANCE(?,?,0.005) FROM DUAL";
    public static double getMinDistance(Node n1, Node n2)
            throws SQLException, JGeometryToShapeException, DatabaseHandler.LoginException
    {
        PreparedStatement pstmt = DatabaseHandler.getInstance().getConnection().prepareStatement(SQL_GET_MIN_DISTANCE);
        STRUCT obj = JGeometry.store(shapeToJGeometry(n1.shape), DatabaseHandler.getInstance().getConnection());
        pstmt.setObject(1, obj);
        STRUCT obj2 = JGeometry.store(shapeToJGeometry(n2.shape), DatabaseHandler.getInstance().getConnection());
        pstmt.setObject(2, obj2);
        var resultSet = pstmt.executeQuery();

        double distance = 0;
        while (resultSet.next()) {
            distance = resultSet.getDouble(1);
        }

        return distance;
    }

    public boolean imageUpdated()
    {
        return imgUpdated;
    }

    public BufferedImage getImage()
    {
        return img;
    }

    public void setImage(BufferedImage img)
    {
        imgUpdated = true;
        this.img = img;
    }

    public Integer ID()
    {
        return ID;
    }

    private static Shape point2dToShape(Point2D point2D)
    {
        return new Rectangle2D.Double(point2D.getX() - 0.5, point2D.getY() - 0.5, 1, 1);
    }

    private static Shape jGeometryToShape(JGeometry jGeometry) throws Node.JGeometryToShapeException
    {
        // check a type of JGeometry object
        switch (jGeometry.getType()) {
            // return a shape for non-points
            case JGeometry.GTYPE_CURVE:
            case JGeometry.GTYPE_POLYGON:
            case JGeometry.GTYPE_COLLECTION:
            case JGeometry.GTYPE_MULTICURVE:
            case JGeometry.GTYPE_MULTIPOLYGON:
                return jGeometry.createShape();
            // return a rectangular "point" for a point (centered over the point location with unit dimensions)
            case JGeometry.GTYPE_POINT:
                return point2dToShape(jGeometry.getJavaPoint());
            // return an area of rectangular "points" for all points (each centered over the points location with unit dimensions)
            case JGeometry.GTYPE_MULTIPOINT:
                final Area area = new Area();
                for (Point2D point2D : jGeometry.getJavaPoints()) {
                    area.add(new Area(point2dToShape(point2D)));
                }
                return area;
        }
        // it is something else (we do not know how to convert)
        throw new JGeometryToShapeException();
    }

    private static final String SQL_SELECT_ALL = "SELECT n.shape.Get_WKT(), n.shape, n.id, n.title, n.text, " +
            "n.color, n.diagram_id, n.image FROM nodes n WHERE n.diagram_id = ?";
    public static List<Node> fetchNodes(Integer diagramId) throws SQLException, DatabaseHandler.LoginException
    {
        final List<Node> shapeList = new LinkedList<>();

        try (PreparedStatement stmt = DatabaseHandler.getInstance().getConnection().prepareStatement(SQL_SELECT_ALL)) {
            stmt.setInt(1, diagramId);
            try (ResultSet resultSet = stmt.executeQuery()) {
                while (resultSet.next()) {
                    shapeList.add(fetchNode((OracleResultSet) resultSet));
                }
                return Collections.unmodifiableList(shapeList);
            }
        }
    }

    private static Node fetchNode(OracleResultSet rs) throws SQLException
    {
        final String wkt = rs.getString(1);
        final byte[] image = rs.getBytes(2);

        System.out.println("loading " + wkt + " ...");

        Node node = new Node();

        node.ID = rs.getInt(3);
        node.title = rs.getString(4);
        node.text = rs.getString(5);
        node.color = new Color(rs.getInt(6));
        node.diagramID = rs.getInt(7);
        node.shape = loadShapeFromImage(image);

        try {
            OrdImage imgProxy = (OrdImage) rs.getORAData(8, OrdImage.getORADataFactory());
            if (imgProxy != null) {
                imgProxy.getDataInFile("./.user-data-node" + node.ID + "-pic.png");
                node.img = ImageIO.read(new File("./.user-data-node"+node.ID+"-pic.png"));
            }
        }
        catch (Exception ignored) {
        }

        return node;
    }

    private static Shape loadShapeFromImage(final byte[] image) throws SQLException
    {
        try {
            final JGeometry jGeometry = JGeometry.load(image);
            System.out.println("... loaded as " + jGeometry.toStringFull());

            return jGeometryToShape(jGeometry);
        }
        catch (Exception exception) {
            throw new SQLException("error in JGeometry.load", exception);
        }
    }

    public static boolean isEllipse(Shape shape)
    {
        var pathIt = shape.getPathIterator(null);
        while (!pathIt.isDone()) {
            double[] coords = new double[6];
            var obj = pathIt.currentSegment(coords);
            if (obj == SEG_CLOSE) {
                return false;
            }
            if (obj == SEG_CUBICTO) {
                return true;
            }

            pathIt.next();
        }

        return false;
    }

    public static JGeometry convertEllipse(Shape shape)
    {
        if (!isEllipse(shape)) {
            return null;
        }

        Rectangle2D el = shape.getBounds();
        return new JGeometry(
                JGeometry.GTYPE_POLYGON, 0,
                new int[]{1, 1003, 4},
                new double[]{
                        el.getCenterX(), el.getMinY(),
                        el.getMaxX(), el.getCenterY(),
                        el.getCenterX(), el.getMaxY()});
    }

    public static JGeometry convertRectangle(Shape shape)
    {
        try {
            Rectangle2D.Double el = (Rectangle2D.Double) shape;
            return new JGeometry(
                    JGeometry.GTYPE_POLYGON, 0,
                    new int[]{1, 1003, 3},
                    new double[]{el.getMinX(), el.getMinY(), el.getMaxX(), el.getMaxY()});
        }
        catch (ClassCastException exception) {
            return null;
        }
    }

    public static JGeometry shapeToJGeometry(Shape shape) throws JGeometryToShapeException
    {
        JGeometry converted;
        if ((converted = convertEllipse(shape)) != null) {
            return converted;
        }
        if ((converted = convertRectangle(shape)) != null) {
            return converted;
        }

        try {
            List<Double> points = new LinkedList<>();
            Polygon p = new Polygon();
            var pathIt = shape.getPathIterator(null);
            while (!pathIt.isDone()) {
                double[] coords = new double[4];
                var obj = pathIt.currentSegment(coords);
                if (obj == SEG_CLOSE) {
                    break;
                }
                points.add(coords[0]);
                points.add(coords[1]);

                pathIt.next();
            }

            double[] pointsRaw = new double[points.size()];
            int j = 0;
            for (var i : points) {
                pointsRaw[j++] = i;
            }

            return new JGeometry(
                    JGeometry.GTYPE_POLYGON, 0,
                    new int[]{1, 1003, 1},
                    pointsRaw);
        }
        catch (Exception exception) {
            throw new JGeometryToShapeException();
        }
    }

    private static final String SQL_NODE_BOUND = "SELECT SDO_TUNE.EXTENT_OF('NODES', 'SHAPE', 'TRUE') FROM DUAL";
    public static Rectangle fetchFrameBounds() throws SQLException, DatabaseHandler.LoginException
    {
        PreparedStatement stmt = DatabaseHandler.getInstance().getConnection().prepareStatement(SQL_NODE_BOUND);
        ResultSet resultSet = stmt.executeQuery();
        if (resultSet.next()) {
            final byte[] image = resultSet.getBytes(1);
            return loadShapeFromImage(image).getBounds();
        }
        throw new SQLException("Empty result set");
    }

    private static final String SQL_DIAGRAM_BOUND = "SELECT SDO_AGGR_MBR(SHAPE) FROM NODES where diagram_id = ?";
    public static Rectangle fetchFrameBounds(Integer diagramID) throws SQLException, DatabaseHandler.LoginException
    {
        PreparedStatement stmt = DatabaseHandler.getInstance().getConnection().prepareStatement(SQL_DIAGRAM_BOUND);
        stmt.setInt(1, diagramID);
        ResultSet resultSet = stmt.executeQuery();
        if (resultSet.next()) {
            final byte[] image = resultSet.getBytes(1);
            return loadShapeFromImage(image).getBounds();
        }
        throw new SQLException("Empty result set");
    }

    private static final String SQL_INSERT_NODE = "INSERT INTO nodes (title, text, color, shape, diagram_id) " +
            "values (?, ?, ?, ?, ?)";
    public static void createNode(Node node) throws SQLException, DatabaseHandler.LoginException
    {
        DatabaseHandler dbh = DatabaseHandler.getInstance();

        try (PreparedStatement stmt = dbh.getConnection().prepareStatement(SQL_INSERT_NODE)) {

            stmt.setString(1, node.title);
            stmt.setString(2, node.text);
            stmt.setInt(3, node.color.getRGB());
            STRUCT obj = JGeometry.store(shapeToJGeometry(node.shape), dbh.getConnection());
            stmt.setObject(4, obj);
            stmt.setInt(5, node.diagramID);

            stmt.executeUpdate();
        }
        catch (Exception exception) {
            throw new SQLException("SQLException: unable to create node", exception);
        }
    }

    public enum UpdatedItem
    {
        TITLE,
        TEXT,
        COLOR,
        SHAPE
    }

    private static final String SQL_UPDATE_NODE = "UPDATE nodes SET %s WHERE id = ?";
    private static final String SQL_UPDATE_TITLE = "title = ?";
    private static final String SQL_UPDATE_TEXT = "text = ?";
    private static final String SQL_UPDATE_COLOR = "color = ?";
    private static final String SQL_UPDATE_SHAPE = "shape = ?";

    public static void updateNodes(List<Node> nodes, Set<UpdatedItem> updatedFeatures)
            throws DatabaseHandler.LoginException, SQLException, ShapeOverlapsException
    {
        for (Node node : nodes) {
            updateNode(node, updatedFeatures);
        }
    }

    public static void updateNode(Node node)
            throws DatabaseHandler.LoginException, SQLException, ShapeOverlapsException, IOException
    {
        Set<UpdatedItem> updatedFeatures = new HashSet<>();

        updatedFeatures.add(UpdatedItem.TITLE);
        updatedFeatures.add(UpdatedItem.TEXT);
        updatedFeatures.add(UpdatedItem.COLOR);
        updatedFeatures.add(UpdatedItem.SHAPE);

        if (!node.imgUpdated && node.img != null) {
            if (node.img.getWidth() != 0.9*node.shape.getBounds().width
                    || node.img.getHeight() != 0.8*node.shape.getBounds().height)  {
                node.imgUpdated = true;
            }
        }

        if (node.imageUpdated() && node.getImage() != null) {
            System.out.println("Updating image");
            if (node.img.getWidth() != 0.9*node.shape.getBounds().width
                    || node.img.getHeight() != 0.8*node.shape.getBounds().height) {

                var img =  node.img.getScaledInstance(
                        (int)(0.9*node.shape.getBounds().width),
                        (int)(0.8*node.shape.getBounds().height),
                        Image.SCALE_REPLICATE);

                BufferedImage dimg = new BufferedImage(
                        (int)(0.9*node.shape.getBounds().width),
                        (int)(0.8*node.shape.getBounds().height),
                        BufferedImage.SCALE_SMOOTH);

                Graphics2D g2d = dimg.createGraphics();
                g2d.drawImage(img, 0, 0, null);
                g2d.dispose();

                node.img = dimg;
            }
            uploadImage(node);
            node.imgUpdated = false;
        }

        if (node.imageUpdated() && node.getImage() == null) {
            Node.deleteImageFromDb(node);
        }

        updateNode(node, updatedFeatures);
    }

    public static void updateNode(Node node, Set<UpdatedItem> updatedFeatures)
            throws DatabaseHandler.LoginException, SQLException, ShapeOverlapsException
    {
        DatabaseHandler dbh = DatabaseHandler.getInstance();

        try (PreparedStatement pstmt = dbh.getConnection().prepareStatement(SQL_CAN_UPDATE)) {
            STRUCT obj = JGeometry.store(shapeToJGeometry(node.shape), dbh.getConnection());
            pstmt.setInt(1, node.ID());
            pstmt.setObject(2, obj);
            pstmt.executeQuery();
        }
        catch (Exception exception) {
            throw new ShapeOverlapsException("Nodes should not overlap");
        }

        String updateStatement = createUpdateStatement(updatedFeatures);
        try (PreparedStatement stmt = dbh.getConnection().prepareStatement(updateStatement)) {
            Integer itemIdx = 1;
            for (UpdatedItem ui : updatedFeatures) {
                switch (ui) {
                    case TITLE:
                        stmt.setString(itemIdx, node.title);
                        break;
                    case TEXT:
                        stmt.setString(itemIdx, node.text);
                        break;
                    case COLOR:
                        stmt.setInt(itemIdx, node.color.getRGB());
                        break;
                    case SHAPE:
                        STRUCT obj = JGeometry.store(shapeToJGeometry(node.shape), dbh.getConnection());
                        stmt.setObject(itemIdx, obj);
                        break;
                    default:
                        continue;
                }
                itemIdx++;
            }
            stmt.setInt(itemIdx, node.ID());

            stmt.executeUpdate();
        }
        catch (Exception exception) {
            throw new SQLException("SQLException: Unable to store shape of node", exception);
        }
    }

    public static String createUpdateStatement(Set<UpdatedItem> updatedFeatures)
    {
        StringBuilder featuresBuff = new StringBuilder();
        Integer commaCounter = 1;
        for (UpdatedItem ui : updatedFeatures) {
            switch (ui) {
                case TITLE:
                    featuresBuff.append(SQL_UPDATE_TITLE);
                    break;
                case TEXT:
                    featuresBuff.append(SQL_UPDATE_TEXT);
                    break;
                case COLOR:
                    featuresBuff.append(SQL_UPDATE_COLOR);
                    break;
                case SHAPE:
                    featuresBuff.append(SQL_UPDATE_SHAPE);
                    break;
                default:
                    continue;
            }
            if (commaCounter++ < updatedFeatures.size()) {
                featuresBuff.append(", ");
            }
        }

        StringBuilder statementBuff = new StringBuilder();
        Formatter fmt = new Formatter(statementBuff);
        fmt.format(SQL_UPDATE_NODE, featuresBuff.toString());

        return statementBuff.toString();
    }

    public static class JGeometryToShapeException extends Exception
    {
    }

    public static class ShapeOverlapsException extends Throwable
    {
        public ShapeOverlapsException(String shapes_overlap)
        {
            super(shapes_overlap);
        }
    }

    public static void uploadImage (Node node) throws DatabaseHandler.LoginException, SQLException, IOException
    {
        File f = new File("./.user-data-node" + node.ID + "-pic.png");
        ImageIO.write(node.img, "png", f);

        var conn = DatabaseHandler.getInstance().getConnection();
        boolean originalAutoCommit = conn.getAutoCommit();
        conn.setAutoCommit(false);

        Statement stmt1 = conn.createStatement();
        String insertSql = "UPDATE nodes SET image = ordsys.ordimage.init() WHERE id = " + node.ID();
        stmt1.executeUpdate(insertSql);
        stmt1.close();

        Statement stmt2 = conn.createStatement();
        String selSql = "SELECT image FROM nodes WHERE id = "+node.ID()+" for update";
        OracleResultSet rset = (OracleResultSet) stmt2.executeQuery(selSql);
        rset.next();
        OrdImage imgProxy = (OrdImage)
                rset.getORAData("image", OrdImage.getORADataFactory());
        rset.close();
        stmt2.close();

        imgProxy.loadDataFromFile(f.getAbsolutePath());
        imgProxy.setProperties();

        String updateSql1 = "UPDATE nodes SET image = ? WHERE id ="+node.ID();
        OraclePreparedStatement pstmt = (OraclePreparedStatement)
                conn.prepareStatement(updateSql1);
        pstmt.setORAData(1, imgProxy);
        pstmt.executeUpdate();
        pstmt.close();

        // Here we can do the settings of the average color, histogram, etc.

        conn.commit();

        conn.setAutoCommit(originalAutoCommit);
    }
}
