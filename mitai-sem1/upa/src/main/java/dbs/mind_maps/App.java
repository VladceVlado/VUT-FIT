package dbs.mind_maps;

import dbs.mind_maps.controllers.LoginScreen;
import dbs.mind_maps.controllers.ScreenManager;


public class App
{
    public static void main(String[] args)
    {
        new ScreenManager(new LoginScreen());
    }
}
