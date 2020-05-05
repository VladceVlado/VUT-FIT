package dbs.mind_maps.views;

import dbs.mind_maps.models.DatabaseHandler;
import dbs.mind_maps.models.Node;

import javax.imageio.ImageIO;
import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.awt.geom.AffineTransform;
import java.awt.geom.Ellipse2D;
import java.awt.geom.Rectangle2D;
import java.awt.image.BufferedImage;
import java.io.File;
import java.sql.SQLException;
import java.util.LinkedList;
import java.util.List;


public class NodeView extends JPanel
{
    private static final String EDIT_TITLE = "Edit title";
    private static final String EDIT_COLOR = "Edit color";
    private static final String EDIT_SHAPE = "Edit shape";
    private static final String ADD_IMAGE = "Add image";
    private static final String DELETE_IMAGE = "Delete image";
    private static final String DELETE_NODE = "Delete node";

    private static final String RECTANGLE = "Rectangle";
    private static final String RHOMBOID = "Rhomboid";
    private static final String OCTAGON = "Octagon";
    private static final String CIRCLE = "Circle";

    final JFileChooser fc = new JFileChooser();

    private JLabel title;
    private JTextArea text;
    private JScrollPane textWrap;

    public Node node;
    private ImageIcon nodeImg = null;
    private JLabel imgLabel = null;

    private Point mouseLastSeen = null;

    private Point dragAnchor = null;

    private NodesActionListener nodeSelection = null;

    public NodeView(Node node)
    {
        this.node = node;

        setBounds(node.shape.getBounds());
        setLayout(new GridLayout(0, 1));

        setComponentPopupMenu(new NodePopUpMenu());

        addTitle();
        //addText();
        MouseOnViewListener mov = new MouseOnViewListener();

        if (node.getImage() != null) {
            nodeImg = new ImageIcon(node.getImage());
            imgLabel = new JLabel(nodeImg);
        }
        else
        {
            imgLabel = new JLabel();
            imgLabel.setVisible(false);
        }

        add(imgLabel);

        addMouseListener(mov);
        addMouseMotionListener(mov);
    }

    public void addNodeSelectionListener(NodesActionListener nsl)
    {
        nodeSelection = nsl;
    }

    private class MouseOnViewListener extends MouseAdapter implements MouseMotionListener
    {
        private void finishMoving()
        {
            setCursor(new Cursor(Cursor.DEFAULT_CURSOR));
            dragAnchor = null;
            setVisible(true);
        }

        @Override
        public void mouseClicked(MouseEvent e)
        {
            mouseLastSeen = null;
            List<Node> selection = new LinkedList<Node>();
            selection.add(node);
            nodeSelection.selectedNodes(selection);
        }

        @Override
        public void mousePressed(MouseEvent e)
        {
            mouseLastSeen = null;
            if (nodeSelection.prepareMoveNode(node)) {
                setCursor(new Cursor(Cursor.HAND_CURSOR));
                dragAnchor = e.getPoint();
                setVisible(false);
            }
        }

        @Override
        public void mouseReleased(MouseEvent mouseEvent)
        {
            mouseLastSeen = null;
            nodeSelection.updateNode(node);
            finishMoving();
        }

        @Override
        public void mouseDragged(MouseEvent e)
        {
            mouseLastSeen = null;
            if (dragAnchor == null) {
                return;
            }
            Point actualPoint = e.getPoint();

            double xDiff = (actualPoint.getX()-dragAnchor.getX());
            double yDiff = (actualPoint.getY()-dragAnchor.getY());

            AffineTransform at = new AffineTransform();
            at.setToTranslation(xDiff, yDiff);
            node.shape = at.createTransformedShape(node.shape);

            dragAnchor = actualPoint;
        }

        @Override
        public void mouseExited(MouseEvent e) {
            mouseLastSeen = null;
        }

        @Override
        public void mouseMoved(MouseEvent e) {
            mouseLastSeen = e.getPoint();
        }
    }

    private void addText()
    {
        text = new JTextArea(2, 1);
        text.setEditable(true);
        text.setText(node.text);
        textWrap = new JScrollPane(text);
        add(textWrap);
    }

    private void addTitle()
    {
        JTextArea titleEditable = new JTextArea(1, 1);
        title = new JLabel(node.title, SwingConstants.CENTER);
        add(title);
    }

    @Override
    public void paint(Graphics g)
    {
        if (node.resized) {
            setVisible(false);
        }

        setBounds(node.shape.getBounds());

        title.setText(node.title);
        super.paint(g);
    }

    @Override
    protected void paintComponent(Graphics g)
    {
    }

    @Override
    protected void paintBorder(Graphics g)
    {
    }

    private class NodePopUpMenu extends JPopupMenu
    {
        public JMenuItem editTitle = new JMenuItem(EDIT_TITLE);
        public JMenuItem editColor = new JMenuItem(EDIT_COLOR);
        public JMenuItem editShape = new JMenuItem(EDIT_SHAPE);
        public JMenuItem addImage = new JMenuItem(ADD_IMAGE);
        public JMenuItem deleteImage = new JMenuItem(DELETE_IMAGE);
        public JMenuItem deleteNode = new JMenuItem(DELETE_NODE);

        public NodePopUpMenu()
        {
            PopUpListener popUpListener = new PopUpListener();

            editTitle.addActionListener(popUpListener);
            editColor.addActionListener(popUpListener);
            editShape.addActionListener(popUpListener);
            addImage.addActionListener(popUpListener);
            deleteImage.addActionListener(popUpListener);
            deleteNode.addActionListener(popUpListener);

            add(editTitle);
            add(editColor);
            add(editShape);
            add(addImage);
            add(deleteImage);
            add(deleteNode);
        }
    }

    private class PopUpListener implements ActionListener
    {
        public PopUpListener()
        {
        }

        @Override
        public void actionPerformed(ActionEvent event)
        {
            String menuItem = event.getActionCommand();

            switch (menuItem) {
                case EDIT_TITLE:
                    editTitle();
                    break;

                case EDIT_COLOR:
                    editColor();
                    break;

                case EDIT_SHAPE:
                    editShape();
                    break;

                case ADD_IMAGE:
                    addImage();
                    break;

                case DELETE_IMAGE:
                    deleteImage(node);
                    break;

                case DELETE_NODE:
                    nodeSelection.deleteNode(node);
                    break;
            }
        }

        private void editTitle()
        {
            node.title = MessageDialog.SingleInput.get("Fill the following field", "Title", node.title);
            nodeSelection.updateNode(node);
        }

        private void editColor()
        {
            node.color = JColorChooser.showDialog(null, "Choose a color", node.color);
            nodeSelection.updateNode(node);
        }

        private void addImage()
        {
            int returnVal = fc.showOpenDialog(NodeView.this);

            if (returnVal == JFileChooser.APPROVE_OPTION) {
                try {
                    File f = fc.getSelectedFile();
                    BufferedImage img = ImageIO.read(f);
                    var sh = node.shape.getBounds();
                    img.getScaledInstance(sh.width, sh.height, Image.SCALE_SMOOTH);
                    node.setImage(img);
                    nodeSelection.updateNode(node);
                }
                catch (Exception exception) {
                    MessageDialog.error("Load picture error", "Unable to load provided picture.");
                }
            }
        }

        private void deleteImage(Node node)
        {
            node.setImage(null);
            nodeSelection.updateNode(node);
        }

        private void editShape()
        {
            // Triangle is a problem because of a bug in some trigger in db
            var shapes = new String[]{RECTANGLE, RHOMBOID, OCTAGON, CIRCLE};

            int x = (int) Math.round(node.shape.getBounds().getMinX());
            int y = (int) Math.round(node.shape.getBounds().getMinY());
            int width = Math.round(node.shape.getBounds().width);
            int height = Math.round(node.shape.getBounds().height);

            Object selected = JOptionPane.showInputDialog(null, null,
                    "Select shape", JOptionPane.QUESTION_MESSAGE, null, shapes, null);

            if (selected != null) {

                switch (selected.toString()) {
                    case RECTANGLE:
                        node.shape = createRectangle(x, y, width, height);
                        break;

                    case RHOMBOID:
                        node.shape = createRhomboid(x, y, Math.min(height, width));
                        break;

                    case OCTAGON:
                        node.shape = createOctagon(x, y, Math.min(height, width));
                        break;

                    case CIRCLE:
                        node.shape = createCircle(x, y, Math.min(height, width));
                    break;
                }
            }

            nodeSelection.updateNode(node);
        }
    }

    public static Rectangle2D createRectangle(int x, int y, int width, int height)
    {
        return new Rectangle2D.Double(x, y, width, height);
    }

    public static Polygon createRhomboid(int x, int y, int len)
    {
        int half = len / 2;
        Polygon rhomboid = new Polygon();
        rhomboid.addPoint(x, y - half);
        rhomboid.addPoint(x + half, y);
        rhomboid.addPoint(x + len, y - half);
        rhomboid.addPoint(x + half, y - len);
        rhomboid.addPoint(x, y - half);
        rhomboid.translate(x - rhomboid.getBounds().x, y - rhomboid.getBounds().y);
        return rhomboid;
    }

    public static Polygon createOctagon(int x, int y, int len)
    {
        int third = len / 3;
        int twoThird = len * 2 / 3;
        Polygon hexagon = new Polygon();
        hexagon.addPoint(x, y - twoThird);
        hexagon.addPoint(x, y - third);
        hexagon.addPoint(x + third, y);
        hexagon.addPoint(x + twoThird, y);
        hexagon.addPoint(x + len, y - third);
        hexagon.addPoint(x + len, y - twoThird);
        hexagon.addPoint(x + twoThird, y - len);
        hexagon.addPoint(x + third, y - len);
        hexagon.addPoint(x, y - twoThird);
        hexagon.translate(x - hexagon.getBounds().x, y - hexagon.getBounds().y);
        return hexagon;
    }

    public static Ellipse2D createCircle(int x, int y, int len)
    {
        return new Ellipse2D.Double(x, y, len, len);
    }
}
