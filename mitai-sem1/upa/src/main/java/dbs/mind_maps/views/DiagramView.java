package dbs.mind_maps.views;

import dbs.mind_maps.controllers.DiagramScreen;
import dbs.mind_maps.models.DiagramModel;

import javax.swing.*;
import javax.swing.border.Border;
import javax.swing.border.EmptyBorder;
import java.awt.*;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;


public class DiagramView
{
    private static final short PADDING_TOP_BOT = 5;
    private static final short PADDING_RIGHT_LEFT = 20;
    private static final Color COLOR_DEFAULT = Color.LIGHT_GRAY;
    private static final Color COLOR_SELECTED = Color.CYAN;

    private DiagramScreen diagramScreen;

    public JPanel panel = new JPanel();
    public DiagramModel diagramModel;
    public boolean selected = false;

    public DiagramView(DiagramScreen diagramScreen, DiagramModel diagramModel)
    {
        this.diagramScreen = diagramScreen;
        this.diagramModel = diagramModel;

        Border border = BorderFactory.createLineBorder(Color.black);
        GridLayout layout = new GridLayout(4, 1);

        panel.setLayout(layout);
        panel.setBorder(border);
        panel.setBackground(COLOR_DEFAULT);

        JLabel title = new JLabel();
        JLabel name = new JLabel();
        JLabel email = new JLabel();
        JLabel date = new JLabel();

        title.setHorizontalAlignment(SwingConstants.CENTER);
        name.setHorizontalAlignment(SwingConstants.CENTER);
        email.setHorizontalAlignment(SwingConstants.CENTER);
        date.setHorizontalAlignment(SwingConstants.CENTER);

        title.setBorder(new EmptyBorder(PADDING_TOP_BOT, PADDING_RIGHT_LEFT, PADDING_TOP_BOT, PADDING_RIGHT_LEFT));
        name.setBorder(new EmptyBorder(PADDING_TOP_BOT, PADDING_RIGHT_LEFT, PADDING_TOP_BOT, PADDING_RIGHT_LEFT));
        email.setBorder(new EmptyBorder(PADDING_TOP_BOT, PADDING_RIGHT_LEFT, PADDING_TOP_BOT, PADDING_RIGHT_LEFT));
        date.setBorder(new EmptyBorder(PADDING_TOP_BOT, PADDING_RIGHT_LEFT, PADDING_TOP_BOT, PADDING_RIGHT_LEFT));

        title.setText(diagramModel.title);
        name.setText(diagramModel.name);
        email.setText(diagramModel.email);
        date.setText(diagramModel.date);

        panel.add(title);
        panel.add(name);
        panel.add(email);
        panel.add(date);

        MyMouseListener mouseListener = new MyMouseListener(this);
        panel.addMouseListener(mouseListener);
    }

    public void makeSelected()
    {
        panel.setBackground(COLOR_SELECTED);
        selected = true;
    }

    public void makeUnselected()
    {
        panel.setBackground(COLOR_DEFAULT);
        selected = false;
    }

    private class MyMouseListener implements MouseListener
    {
        private DiagramView diagramView;

        public MyMouseListener(DiagramView diagramView)
        {
            this.diagramView = diagramView;
        }

        @Override
        public void mouseClicked(MouseEvent event)
        {
            if (event.getClickCount() == 2) {
                diagramScreen.diagramDoubleCliked(diagramView);
            }
        }

        @Override
        public void mousePressed(MouseEvent event)
        {
            diagramScreen.diagramPressed(diagramView);
        }

        @Override
        public void mouseReleased(MouseEvent event)
        {
        }

        @Override
        public void mouseEntered(MouseEvent event)
        {
        }

        @Override
        public void mouseExited(MouseEvent event)
        {
        }
    }
}
