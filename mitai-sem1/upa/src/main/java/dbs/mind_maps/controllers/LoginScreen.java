package dbs.mind_maps.controllers;

import dbs.mind_maps.models.DatabaseHandler;
import dbs.mind_maps.views.MessageDialog;

import javax.swing.*;
import javax.swing.border.EmptyBorder;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;


public class LoginScreen implements ScreenInterface
{
    private static final String DEFAULT_URL = "jdbc:oracle:thin:@//gort.fit.vutbr.cz:1521/orclpdb";
    private static final short PADDING = 20;
    private static final Color COLOR = Color.LIGHT_GRAY;

    private JMenu mainMenu = new JMenu(MAIN_MENU);
    private JPanel panel = new JPanel();
    private JTextField fieldUrl = new JTextField();
    private JTextField fieldUsername = new JTextField();
    private JPasswordField fieldPassword = new JPasswordField();

    public LoginScreen()
    {
    }

    @Override
    public void initView(ScreenManager screenManager)
    {
        // --- Menu section --- //

        JMenuItem help = new JMenuItem(HELP);
        help.addActionListener(actionEvent -> help());
        mainMenu.add(help);

        // --- Frame content section --- //

        screenManager.frame.setLayout(new GridBagLayout());

        panel.setLayout(new GridLayout(2,1, PADDING, PADDING / 2));
        panel.setPreferredSize(new Dimension(500, 250));
        panel.setBackground(COLOR);
        panel.setBorder(BorderFactory.createLineBorder(Color.black));

        JPanel innerPanel1 = new JPanel();
        innerPanel1.setLayout(new GridLayout(3, 2, PADDING, PADDING / 2));
        innerPanel1.setBorder(new EmptyBorder(PADDING, PADDING, PADDING / 2, PADDING));
        innerPanel1.setBackground(COLOR);

        JPanel innerPanel2 = new JPanel();
        innerPanel2.setBorder(new EmptyBorder(PADDING / 2, PADDING, PADDING, PADDING));
        innerPanel2.setBackground(COLOR);

        JLabel labelUrl = new JLabel("URL:");
        fieldUrl.setText(DEFAULT_URL);
        JLabel labelUsername = new JLabel("Username:");
        JLabel labelPassword = new JLabel("Password:");
        JButton buttonLogin = new JButton(LOGIN);
        buttonLogin.setPreferredSize(new Dimension(250, 50));
        buttonLogin.addActionListener(new ButtonListener(screenManager));

        innerPanel1.add(labelUrl);
        innerPanel1.add(fieldUrl);
        innerPanel1.add(labelUsername);
        innerPanel1.add(fieldUsername);
        innerPanel1.add(labelPassword);
        innerPanel1.add(fieldPassword);

        innerPanel2.add(buttonLogin);

        panel.add(innerPanel1);
        panel.add(innerPanel2);

        placeComponents(screenManager.frame);

        // For debug purpose when you want to skip login screen, see Makefile
        if (System.getProperty("build_type").equals("debug")) {
            fieldUsername.setText(System.getProperty("username"));
            fieldPassword.setText(System.getProperty("password"));
        }
    }

    @Override
    public void placeComponents(JFrame frame)
    {
        frame.getJMenuBar().add(mainMenu);
        frame.add(panel);
    }

    public class ButtonListener implements ActionListener
    {
        private ScreenManager screenManager;

        ButtonListener(ScreenManager screenManager)
        {
            this.screenManager = screenManager;
        }

        @Override
        public void actionPerformed(ActionEvent event)
        {
            String url = fieldUrl.getText();
            String username = fieldUsername.getText();
            String password = String.valueOf(fieldPassword.getPassword());

            try {
                DatabaseHandler.login(url, username, password);
                screenManager.replaceAllScreensWith(new DiagramScreen());
            }
            catch (DatabaseHandler.LoginException exception) {
                System.err.println(exception.getMessage());
                MessageDialog.error("Error", "Logon denied");
            }
        }
    }

    private void help()
    {
        MessageDialog.info(ScreenInterface.HELP, "Provide your credentials and log in to the databse server");
    }
}
