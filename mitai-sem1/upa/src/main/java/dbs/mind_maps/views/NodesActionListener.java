package dbs.mind_maps.views;

import dbs.mind_maps.models.Node;

import java.awt.*;
import java.util.List;


public interface NodesActionListener
{
    void selectedNodes(List<Node> nodes);
    void createNode(Node node);
    void updateNode(Node node);
    void deleteNode(Node node);

    default void moveNode(Node node, Point location)
    {
    }

    default boolean prepareMoveNode(Node node)
    {
        return true;
    }

    default boolean canResizeNode(Node node)
    {
        return true;
    }
}
