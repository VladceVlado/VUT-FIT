package dbs.mind_maps.controllers;

import dbs.mind_maps.models.DatabaseHandler;
import dbs.mind_maps.models.Node;
import dbs.mind_maps.views.MessageDialog;
import dbs.mind_maps.views.NodesActionListener;
import dbs.mind_maps.views.NodePanel;
import dbs.mind_maps.views.NodeView;

import java.awt.*;
import java.awt.event.*;
import java.io.IOException;
import java.sql.SQLException;

import javax.swing.*;

import java.util.LinkedList;
import java.util.List;


public class NodeScreen implements ScreenInterface
{
    private static final String BACK = "Back to Mind Maps";

    private FrameListener frameListener = null;
    private JMenu mainMenu = new JMenu(MAIN_MENU);
    private NodePanel nodePanel;
    private Integer id;
    private JFrame screen = null;

    private int minNodeX = Integer.MAX_VALUE;
    private int minNodeY = Integer.MAX_VALUE;

    public NodeScreen(Integer id)
    {
        this.id = id;
    }

    @Override
    public void initView(ScreenManager screenManager)
    {
        // --- Menu section --- //

        var back = new JMenuItem(BACK);
        var help = new JMenuItem(HELP);
        var logout = new JMenuItem(LOGOUT);

        back.addActionListener(actionEvent -> screenManager.showPreviousScreen());
        logout.addActionListener(actionEvent -> screenManager.replaceAllScreensWith(new LoginScreen()));
        help.addActionListener(actionEvent -> help());

        mainMenu.add(back);
        mainMenu.add(help);
        mainMenu.add(logout);

        // --- Frame content section --- //

        nodePanel = new NodePanel();
        nodePanel.addDiagramActionListener(new NodeDiagramListener());

        frameListener = new FrameListener(screenManager.frame);

        placeComponents(screenManager.frame);
    }

    @Override
    public void placeComponents(JFrame frame)
    {
        try {
            Rectangle rect = Node.fetchFrameBounds(id);
            
            int widthWithNodes = (rect.width + minNodeX) * 10 / 9;
            int heightWithNodes = (rect.height + minNodeY) * 10 / 9;

            if (widthWithNodes > frame.getWidth() && heightWithNodes > frame.getHeight()) {
                frame.setSize(widthWithNodes, heightWithNodes);
            }
            else if (widthWithNodes > frame.getWidth()) {
                frame.setSize(widthWithNodes, frame.getHeight());
            }
            else if (heightWithNodes > frame.getHeight()) {
                frame.setSize(frame.getWidth(), heightWithNodes);
            }
        }
        catch (SQLException | DatabaseHandler.LoginException ignored) {
        }

        frame.getJMenuBar().add(mainMenu);
        frame.add(nodePanel);
        frame.addComponentListener(frameListener);
        this.screen = frame;
        updateNodePanel();
    }

    @Override
    public void onExit()
    {
        screen.removeComponentListener(frameListener);
    }

    public void updateNodePanel()
    {
        try {
            minNodeX = Integer.MAX_VALUE;
            minNodeY = Integer.MAX_VALUE;

            final List<Node> nodes = Node.fetchNodes(id);
            List<NodeView> nodeviews = new LinkedList<>();
            for (Node node: nodes) {

                if (node.shape.getBounds().getMinX() < minNodeX) {
                    minNodeX = (int) node.shape.getBounds().getMinX();
                }
                if (node.shape.getBounds().getMinY() < minNodeY) {
                    minNodeY = (int) node.shape.getBounds().getMinY();
                }

                NodeView nv = new NodeView(node);
                nv.addMouseListener(new MouseAdapter() {
                    @Override
                    public void mousePressed(MouseEvent e) {
                        nodePanel.setCursor(new Cursor(Cursor.HAND_CURSOR));
                        nodePanel.repaint();
                    }
                    @Override
                    public void mouseReleased(MouseEvent e) {
                        // TODO: update DB
                        nodePanel.setCursor(new Cursor(Cursor.DEFAULT_CURSOR));
                        nodePanel.repaint();
                    }
                    @Override
                    public void mouseDragged(MouseEvent e) {
                        nodePanel.repaint();
                    }
                });
                nv.addMouseMotionListener(new MouseMotionAdapter() {
                    @Override
                    public void mouseDragged(MouseEvent e) {
                        nodePanel.repaint();
                    }
                });
                nodeviews.add(nv);
            }
            nodePanel.updateNodes(nodeviews);
        }
        catch (SQLException | DatabaseHandler.LoginException ignored) {
        }
    }

    private void createNodeDB(Node node)
    {
        node.diagramID = id;
        try {
            Node.createNode(node);
        }
        catch (SQLException e) {
            MessageDialog.error("Error placing", "Could not place node here: is there a node?");
        }
        catch (Exception ignored) {

        }
    }

    private class NodeDiagramListener implements NodesActionListener
    {
        @Override
        public void selectedNodes(List<Node> nodes)
        {
        }

        @Override
        public void createNode(Node node)
        {
            node.diagramID = id;
            createNodeDB(node);
            updateNodePanel();
        }

        @Override
        public void updateNode(Node node)
        {
            try {
                Node.updateNode(node);
            }
            catch (DatabaseHandler.LoginException | SQLException ignored) {
                ignored.printStackTrace();
            }
            catch (Node.ShapeOverlapsException e) {
                MessageDialog.error("Error move", e.getMessage());
            } catch (IOException e) {
                MessageDialog.error("Unable to update picture", e.getMessage());
            }
            updateNodePanel();
        }

        @Override
        public void deleteNode(Node node)
        {
            try {
                Node.deleteNode(node);
            }
            catch (DatabaseHandler.LoginException | SQLException ignored) {
            }
            updateNodePanel();
        }
    }

    private void help()
    {
        MessageDialog.info(ScreenInterface.HELP, "Create different type of nodes, insert text and " +
                "images to them and connect them togetger.");
    }

    private static class FrameListener implements ComponentListener
    {
        JFrame frame;

        FrameListener(JFrame frame)
        {
            this.frame = frame;
        }

        @Override
        public void componentResized(ComponentEvent arg0)
        {
        }

        @Override
        public void componentMoved(ComponentEvent componentEvent)
        {
        }

        @Override
        public void componentShown(ComponentEvent arg0)
        {
        }

        @Override
        public void componentHidden(ComponentEvent componentEvent)
        {
        }
    }
}
