package dbs.mind_maps.views;

import dbs.mind_maps.models.Node;

import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.awt.geom.AffineTransform;
import java.awt.geom.Rectangle2D;
import java.util.LinkedList;
import java.util.List;
import java.util.Optional;
import java.util.concurrent.ThreadLocalRandom;
import java.util.concurrent.atomic.AtomicBoolean;

import static java.util.Optional.empty;
import static java.util.Optional.of;


public class NodePanel extends JLayeredPane
{
    private static final String DEFAULT_NODE_TITLE = "New node";

    // Shape of a selection area rectangle.
    private Optional<Rectangle> selectedArea = empty();
    // Starting point of a selection.
    private Point selectionAnchorPoint;
    private Point popupAnchor = null;

    private MouseOnDiagramListener diagramMouseAction;
    private NodesActionListener diagramActionListener;
    private NodeActions nodeActionListener;

    public List<Node> nodes;
    public NodePanelpopUpMenu popupMenu;
    private String analytics = "";

    public NodePanel()
    {
        super();
        setLayout(null);

        initPopupMenu();

        diagramMouseAction = new MouseOnDiagramListener();
        diagramActionListener = null;
        nodeActionListener = new NodeActions();

        addMouseListener(diagramMouseAction);
        addMouseMotionListener(diagramMouseAction);

        setComponentPopupMenu(popupMenu);
    }

    private void initPopupMenu()
    {
        popupMenu = new NodePanelpopUpMenu();

        // Create new node
        // Anchor must be updated on mouse events.
        popupMenu.newNode.addActionListener(actionEvent -> {

            if (popupAnchor == null)
                return;

            Node node = new Node();
            node.title = DEFAULT_NODE_TITLE;

            // Get random color
            node.color = new Color((int) (Math.random() * 0x1000000));

            int x = (int) popupAnchor.getX();
            int y = (int) popupAnchor.getY();

            // Get random shape
            switch (ThreadLocalRandom.current().nextInt(1, 5)) {
                case 1:
                    node.shape = NodeView.createRectangle(x, y, 150, 75);
                    break;
                case 2:
                    node.shape = NodeView.createRhomboid(x, y, 100);
                    break;
                case 3:
                    node.shape = NodeView.createOctagon(x, y, 100);
                    break;
                case 4:
                    node.shape = NodeView.createCircle(x, y, 100);
                    break;
            }

            diagramActionListener.createNode(node);
        });
    }

    public void addDiagramActionListener(NodesActionListener nsl)
    {
        if (nsl != null)
            diagramActionListener = nsl;
    }

    @Override
    public void paint(Graphics graphics)
    {
        final Graphics2D graphics2D = (Graphics2D) graphics;
        paintNodes(graphics2D);
        super.paint(graphics);

        if (selectedArea.isPresent()) {
            graphics2D.setPaint(Color.BLUE);
            graphics2D.draw(selectedArea.get());
        }

        if (analytics != "") {
            graphics2D.drawString(analytics, 1, 10);
            analytics = "";
        }
    }

    public void updateNodes(List<NodeView> nodes)
    {
        removeAll();
        this.nodes = new LinkedList<>();
        for (NodeView node : nodes) {
            node.addNodeSelectionListener(nodeActionListener);
            add(node);
            this.nodes.add(node.node);
        }
        repaint();
    }

    void paintNodes(Graphics2D graphics2D)
    {
        for (Node node : nodes) {
            paintNode(node, graphics2D);
        }
    }

    public void paintNode(Node node, Graphics2D graphics2D)
    {
        graphics2D.setPaint(node.color);
        graphics2D.fill(node.shape);
        // draw a boundary of the shape
        graphics2D.setPaint(node.borderColor);
        graphics2D.draw(node.shape);
    }

    public void removeSelection()
    {
        // Remove selection area whole.
        selectedArea = empty();
        for (var n: nodes) {
            if (n.selected) {
                n.selected = false;
                n.borderColor = Color.BLACK;
            }
        }
    }

    public void addToSelection(Point point)
    {
        if (selectedArea.isEmpty()) {
            // Save anchor of the selection
            selectionAnchorPoint = point;
            // Create rectangle with one point
            Rectangle rect = new Rectangle(selectionAnchorPoint);
            selectedArea = of(rect);
        }
        else {
            Rectangle rect = new Rectangle(selectionAnchorPoint);
            rect.add(point);
            // Create new selection area.
            selectedArea = of(rect);
        }
        for (var n: nodes) {
            if (n.shape.intersects(selectedArea.get())
                    || n.shape.intersects(selectedArea.get())) {
                n.selected = true;
                n.borderColor = Color.BLUE;
            }
            else {
                n.selected = false;
                n.borderColor = Color.BLACK;
            }
        }
    }

    public List<Node> getSelection()
    {
        List<Node> selection = new LinkedList<>();

        for (Node node: nodes) {
            if (node.selected) {
                selection.add(node);
            }
        }

        return selection;
    }

    public static class NodePanelpopUpMenu extends JPopupMenu
    {
        public JMenuItem newNode;

        public NodePanelpopUpMenu() {
            newNode = new JMenuItem("Insert new node");
            add(newNode);
        }
    }

    private class MouseOnDiagramListener extends MouseAdapter implements MouseMotionListener
    {
        @Override
        public void mouseClicked(MouseEvent mouseEvent)
        {
            popupAnchor = mouseEvent.getPoint();
            removeSelection();
            repaint();
        }

        @Override
        public void mousePressed(MouseEvent mouseEvent)
        {
            popupAnchor = mouseEvent.getPoint();

            if (resizeSelection != null) {
                resizeAnchor = mouseEvent.getPoint();
                resizeSelection.resized = true;
                removeSelection();

                AffineTransform at = new AffineTransform();
                at.setToScale(0.99, 0.99);
                resizeSelection.shape = at.createTransformedShape(resizeSelection.shape);

                repaint();
                return;
            }
            removeSelection();
            addToSelection(mouseEvent.getPoint());

            repaint();
        }

        @Override
        public void mouseReleased(MouseEvent mouseEvent)
        {
            popupAnchor = mouseEvent.getPoint();

            if (resizeSelection != null) {
                nodeActionListener.updateNode(resizeSelection);
                resizeSelection.borderColor = resizeSelection.selected? Color.BLUE: Color.BLACK;
                resizeSelection.resized = false;
                resizeSelection = null;
                repaint();
                return;
            }
            addToSelection(mouseEvent.getPoint());

            try {
                var nselection = getSelection();
                if (!nselection.isEmpty()) {

                    if (nselection.size() == 2) {
                        analytics = String.format("Area: %.2f; Min distance: %.2f",
                                Node.getSelectedArea(selectedArea.get()),
                                Node.getMinDistance(nselection.get(0), nselection.get(1)));
                    }
                    else {
                        analytics = String.format("Area: %.2f",
                                Node.getSelectedArea(selectedArea.get()));
                    }
                }
            }
            catch (Exception ignored) {
            }

            selectedArea = empty();

            diagramActionListener.selectedNodes(getSelection());

            repaint();
        }

        @Override
        public void mouseDragged(MouseEvent mouseEvent)
        {
            if (resizeSelection != null) {
                Point actualPoint = mouseEvent.getPoint();

                AffineTransform at = new AffineTransform();

                var w = resizeSelection.shape.getBounds().width;
                var h = resizeSelection.shape.getBounds().height;

                double xScale = (w+actualPoint.getX()-resizeAnchor.getX())/w;
                double yScale = (h+actualPoint.getY()-resizeAnchor.getY())/h;

                double origX = resizeSelection.shape.getBounds().x;
                double origY = resizeSelection.shape.getBounds().y;

                at.setToScale(xScale, yScale);
                resizeSelection.shape = at.createTransformedShape(resizeSelection.shape);

                double newX = resizeSelection.shape.getBounds().x;
                double newY = resizeSelection.shape.getBounds().y;

                at.setToTranslation(origX-newX, origY-newY);
                resizeSelection.shape = at.createTransformedShape(resizeSelection.shape);

                resizeAnchor = actualPoint;

                repaint();
                return;
            }
            addToSelection(mouseEvent.getPoint());

            repaint();
        }

        Node resizeSelection = null;
        Point resizeAnchor = null;

        @Override
        public void mouseMoved(MouseEvent mouseEvent)
        {
            setCursor(new Cursor(Cursor.DEFAULT_CURSOR));

            AtomicBoolean updated = new AtomicBoolean(false);
            if (resizeSelection != null) {
                resizeSelection.borderColor = resizeSelection.selected? Color.BLUE: Color.BLACK;
                resizeSelection = null;
                updated.set(true);
            }

            Point point2D = mouseEvent.getPoint();
            Rectangle2D.Double mouseRect = new Rectangle2D.Double(point2D.getX() - 5, point2D.getY() - 5, 13, 13);

            for (var n: nodes) {
                if (n.shape.intersects(mouseRect)) {
                    setCursor(new Cursor(Cursor.W_RESIZE_CURSOR));
                    n.borderColor = Color.RED;
                    resizeSelection = n;
                    updated.set(true);
                    break;
                }
            }

            if (updated.get()) {
                repaint();
            }
        }

        @Override
        public void mouseEntered(MouseEvent mouseEvent)
        {
            if (resizeSelection != null) {
                resizeSelection.borderColor = resizeSelection.selected? Color.BLUE: Color.BLACK;
                resizeSelection = null;
            }
            repaint();
        }

        @Override
        public void mouseExited(MouseEvent mouseEvent)
        {
            setCursor(new Cursor(Cursor.DEFAULT_CURSOR));
            if (resizeSelection != null) {
                resizeSelection.borderColor = resizeSelection.selected? Color.BLUE: Color.BLACK;
                resizeSelection = null;
            }
            repaint();
        }
    }

    class NodeActions implements NodesActionListener
    {
        @Override
        public void selectedNodes(List<Node> nodes)
        {
            removeSelection();
            for (var node: nodes) {
                node.selected = true;
                node.borderColor = Color.BLUE;
            }
            repaint();
            diagramActionListener.selectedNodes(nodes);
        }

        @Override
        public void createNode(Node node)
        {
            // This will not be called here.
        }

        @Override
        public void updateNode(Node node)
        {
            diagramActionListener.updateNode(node);
        }

        @Override
        public void deleteNode(Node node)
        {
            diagramActionListener.deleteNode(node);
        }

        @Override
        public boolean prepareMoveNode(Node node)
        {
            removeSelection();
            node.selected = true;
            node.borderColor = Color.BLUE;

            return true;
        }

        @Override
        public boolean canResizeNode(Node node)
        {
            return true;
        }
    }
}
