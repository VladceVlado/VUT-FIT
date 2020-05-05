package dbs.mind_maps.controllers;

import dbs.mind_maps.models.DatabaseHandler;

import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.sql.SQLException;
import java.util.ArrayDeque;


public class ScreenManager
{
    private ArrayDeque<ScreenInterface> screenStack = new ArrayDeque<>();
    private WindowListener closeListener = new CloseListener();

    public JFrame frame = new JFrame();
    public JMenuBar menuBar = new JMenuBar();

    public ScreenManager(ScreenInterface baseView)
    {
        frame.setTitle(ScreenInterface.TITLE);
        frame.setMinimumSize(new Dimension(ScreenInterface.MINIMUM_WIDTH, ScreenInterface.MINIMUM_HEIGHT));
        frame.setSize(ScreenInterface.MINIMUM_WIDTH, ScreenInterface.MINIMUM_HEIGHT);
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.addWindowListener(closeListener);
        frame.setLocationRelativeTo(null);
        frame.setJMenuBar(menuBar);

        replaceAllScreensWith(baseView);
    }

    private void reinitFrame()
    {
        frame.getJMenuBar().removeAll();
        frame.getContentPane().removeAll();
        frame.setVisible(true);
        frame.setLayout(new BorderLayout());
    }

    public void replaceScreenWith(ScreenInterface newScreen)
    {
        ScreenInterface actualScreen = screenStack.pop();
        actualScreen.onExit();
        reinitFrame();
        screenStack.push(newScreen);
        newScreen.initView(this);
        drawScreen();
    }

    public void replaceAllScreensWith(ScreenInterface newScreen)
    {
        while (!screenStack.isEmpty()) {
            screenStack.peek().onExit();
            screenStack.pop();
        }
        reinitFrame();
        screenStack.push(newScreen);
        newScreen.initView(this);
        drawScreen();
    }

    public void showNextScreen(ScreenInterface newScreen)
    {
        if (screenStack.isEmpty()) {
            return;
        }

        ScreenInterface actualScreen = screenStack.peek();
        actualScreen.onSuspend();
        reinitFrame();
        screenStack.push(newScreen);
        newScreen.initView(this);
        drawScreen();
    }

    public void showPreviousScreen()
    {
        if (screenStack.size() > 1) {
            screenStack.peek().onSuspend();
            screenStack.pop();
            assert screenStack.peek() != null;
            screenStack.peek().onResume();
            reinitFrame();
            drawScreen();
        }
        else {
            exit();
        }
    }

    private void drawScreen()
    {
        if (screenStack.isEmpty()) {
            return;
        }
        ScreenInterface actualScreen = screenStack.peek();
        actualScreen.placeComponents(frame);
        frame.revalidate();
        frame.repaint();
    }

    public void exit()
    {
        while (!screenStack.isEmpty()) {
            screenStack.peek().onExit();
            screenStack.pop();
        }

        frame.removeWindowListener(closeListener);
        frame.dispose();

        try {
            DatabaseHandler.close();
        }
        catch (SQLException ignored) {
        }
    }

    public class CloseListener extends WindowAdapter
    {
        public void windowClosing(WindowEvent event)
        {
            exit();
        }
    }
}
