package dbs.mind_maps.controllers;

import javax.swing.JFrame;


public interface ScreenInterface
{
    String TITLE = "Mind Maps";
    String MAIN_MENU = "Menu";
    String HELP = "Help";
    String LOGIN = "Log In";
    String LOGOUT = "Log Out";
    short MINIMUM_WIDTH = 800;
    short MINIMUM_HEIGHT = 600;

    void initView(ScreenManager screenManager);

    void placeComponents(JFrame frame);

    default void onSuspend()
    {
    }

    default void onResume()
    {
    }

    default void onExit()
    {
    }
}
