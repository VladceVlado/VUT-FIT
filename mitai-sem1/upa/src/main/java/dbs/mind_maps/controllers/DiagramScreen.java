package dbs.mind_maps.controllers;

import dbs.mind_maps.models.DatabaseHandler;
import dbs.mind_maps.models.DiagramModel;
import dbs.mind_maps.models.Node;
import dbs.mind_maps.views.DiagramView;
import dbs.mind_maps.views.MessageDialog;

import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.io.IOException;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.List;


public class DiagramScreen implements ScreenInterface
{
    private static final String ACTION_MENU = "Actions";
    private static final String INIT_DB = "Initialize database";
    private static final String CLEAN_DB = "Clean database";
    private static final String NEW = "New Mind Map";
    private static final String OPEN = "Open Mind Map";
    private static final String EDIT = "Edit Mind Map";
    private static final String DELETE = "Delete Mind Map";

    private static final short DIAGRAM_ITEM_HEIGHT = 54;
    private static final short SCROLL_PANEL_WIDTH = MINIMUM_WIDTH - MINIMUM_WIDTH / 5;
    private static final short SCROLL_PANEL_HEIGHT = MINIMUM_HEIGHT - MINIMUM_HEIGHT / 5;

    private JMenu mainMenu = new JMenu(MAIN_MENU);
    private JMenu actionMenu = new JMenu(ACTION_MENU);
    private JScrollPane scrollPanel = null;
    private JPanel innerPanel = new JPanel();
    private List<DiagramView> diagramViews = new ArrayList<>();

    public JMenuItem open;  // public because of double click diagram opening

    public DiagramScreen()
    {
    }

    @Override
    public void initView(ScreenManager screenManager)
    {
        // --- Menu section --- //

        MainMenuListener mainMenuListener = new MainMenuListener(screenManager);

        JMenuItem init = new JMenuItem(INIT_DB);
        JMenuItem clean = new JMenuItem(CLEAN_DB);
        JMenuItem help = new JMenuItem(HELP);
        JMenuItem logout = new JMenuItem(LOGOUT);

        init.addActionListener(mainMenuListener);
        clean.addActionListener(mainMenuListener);
        help.addActionListener(mainMenuListener);
        logout.addActionListener(mainMenuListener);

        mainMenu.add(init);
        mainMenu.add(clean);
        mainMenu.add(help);
        mainMenu.add(logout);

        ActionMenuListener actionMenuListener = new ActionMenuListener(screenManager);

        open = new JMenuItem(OPEN);
        JMenuItem create = new JMenuItem(NEW);
        JMenuItem edit = new JMenuItem(EDIT);
        JMenuItem delete = new JMenuItem(DELETE);

        open.addActionListener(actionMenuListener);
        create.addActionListener(actionMenuListener);
        edit.addActionListener(actionMenuListener);
        delete.addActionListener(actionMenuListener);

        actionMenu.add(create);
        actionMenu.add(open);
        actionMenu.add(edit);
        actionMenu.add(delete);

        // --- Frame content section --- //

        FlowLayout flowLayout = new FlowLayout(FlowLayout.LEFT);
        GridLayout gridLayout = new GridLayout(1,1);

        screenManager.frame.setLayout(gridLayout);

        innerPanel.setLayout(flowLayout);
        innerPanel.setPreferredSize(new Dimension(SCROLL_PANEL_WIDTH - SCROLL_PANEL_WIDTH / 10,
                DIAGRAM_ITEM_HEIGHT * diagramViews.size()));
        scrollPanel = new JScrollPane(innerPanel);
        scrollPanel.setPreferredSize(new Dimension(SCROLL_PANEL_WIDTH, SCROLL_PANEL_HEIGHT));

        placeComponents(screenManager.frame);
    }

    @Override
    public void placeComponents(JFrame frame)
    {
        try {
            Rectangle rect = Node.fetchFrameBounds();

            if (rect.width > frame.getWidth() && rect.height > frame.getHeight()) {
                frame.setSize(rect.width, rect.height);
            }
            else if (rect.width > frame.getWidth()) {
                frame.setSize(rect.width, frame.getHeight());
            }
            else if (rect.height > frame.getHeight()) {
                frame.setSize(frame.getWidth(), rect.height);
            }
        }
        catch (SQLException | DatabaseHandler.LoginException ignored) {
        }

        frame.getJMenuBar().add(mainMenu);
        frame.getJMenuBar().add(actionMenu);
        frame.add(scrollPanel);
        refresh();
    }

    public void diagramPressed(DiagramView diagramPressed)
    {
        if (diagramPressed.selected) {
            for (DiagramView diagramView : diagramViews) {
                diagramView.makeUnselected();
            }
        }
        else {
            for (DiagramView diagramView : diagramViews) {
                diagramView.makeUnselected();
            }
            diagramPressed.makeSelected();
        }
    }

    public void diagramDoubleCliked(DiagramView diagramDoubleClicked)
    {
        diagramDoubleClicked.makeSelected();
        open.doClick();
    }

    private void refresh()
    {
        for (DiagramView diagramView : diagramViews) {
            innerPanel.remove(diagramView.panel);
        }

        diagramViews.clear();
        fetchDiagrams();

        for (DiagramView diagramView : diagramViews) {
            innerPanel.add(diagramView.panel);
        }

        innerPanel.validate();
        innerPanel.repaint();
    }

    private void fetchDiagrams()
    {
        try {
            List<DiagramModel> diagramModels = DiagramModel.select();

            for (DiagramModel diagramModel : diagramModels) {
                diagramViews.add(new DiagramView(this, diagramModel));
            }
        }
        catch (SQLException | DatabaseHandler.LoginException exception) {
            System.err.println(exception.getMessage());
        }
    }

    private DiagramView getSelectedDiagram() throws SelectDiagramException
    {
        for (DiagramView diagramView : diagramViews) {
            if (diagramView.selected) {
                return diagramView;
            }
        }
        throw new SelectDiagramException("SelectDiagramException: A diagram has to be selected");
    }

    public class MainMenuListener implements ActionListener
    {
        private ScreenManager screenManager;

        public MainMenuListener(ScreenManager screenManager)
        {
            this.screenManager = screenManager;
        }

        @Override
        public void actionPerformed(ActionEvent event)
        {
            String menuItem = event.getActionCommand();

            switch (menuItem) {
                case INIT_DB:
                    initAction();
                    break;

                case CLEAN_DB:
                    cleanAction();
                    break;

                case HELP:
                    helpAction();
                    break;

                case LOGOUT:
                    logoutAction();
                    break;
            }

            refresh();
        }

        private void initAction()
        {
            try {
                DatabaseHandler.getInstance().dropDb();
                DatabaseHandler.getInstance().createDb();
                DatabaseHandler.getInstance().fillDb();
                refresh();
                MessageDialog.info("Initialize database", "Database was initialized with test examples of mind maps");
            }
            catch (DatabaseHandler.LoginException ignored) {
            }
            catch (IOException exception) {
                MessageDialog.error("Error", "Initialization scripts were not found");
            }
        }

        private void cleanAction()
        {
            try {
                DatabaseHandler.getInstance().dropDb();
                DatabaseHandler.getInstance().createDb();
                refresh();
                MessageDialog.info("Clean database", "Database was cleaned");
            }
            catch (DatabaseHandler.LoginException ignored) {
            }
            catch (IOException exception) {
                MessageDialog.error("Error", "Cleaning scripts were not found");
            }
        }

        private void helpAction()
        {
            MessageDialog.info(ScreenInterface.HELP, "Create your new mind map or work with existing one");
        }

        private void logoutAction()
        {
            screenManager.replaceAllScreensWith(new LoginScreen());
        }
    }

    public class ActionMenuListener implements ActionListener
    {
        private ScreenManager screenManager;

        public ActionMenuListener(ScreenManager screenManager)
        {
            this.screenManager = screenManager;
        }

        @Override
        public void actionPerformed(ActionEvent event)
        {
            String menuItem = event.getActionCommand();

            switch (menuItem) {
                case NEW:
                    createAction();
                    break;

                case OPEN:
                    openAction();
                    break;

                case EDIT:
                    editAction();
                    break;

                case DELETE:
                    deleteAction();
                    break;
            }

            refresh();
        }

        private void createAction()
        {
            try {
                JOptionPaneMultiInput input = new JOptionPaneMultiInput();
                if (!input.title.equals("") && !input.name.equals("")) {
                    DiagramModel.insert(input.title, input.name, input.email);
                }
            }
            catch (MessageDialog.InputException exception) {
                MessageDialog.error("Error", "Title and name are required");
            }
            catch (SQLException | DatabaseHandler.LoginException ignored) {
            }
        }

        private void openAction()
        {
            try {
                DiagramView diagramItem = getSelectedDiagram();
                diagramItem.diagramModel.update(diagramItem.diagramModel.title, diagramItem.diagramModel.name,
                        diagramItem.diagramModel.email);
                screenManager.showNextScreen(new NodeScreen(diagramItem.diagramModel.id));
            }
            catch (SelectDiagramException exception) {
                MessageDialog.error("Error", "A diagram has to be selected");
            }
            catch (DatabaseHandler.LoginException | SQLException ignored) {
            }
        }

        private void editAction()
        {
            try {
                DiagramView diagramItem = getSelectedDiagram();

                JOptionPaneMultiInput input = new JOptionPaneMultiInput(diagramItem.diagramModel.title,
                        diagramItem.diagramModel.name, diagramItem.diagramModel.email);

                diagramItem.diagramModel.update(input.title, input.name, input.email);
            }
            catch (MessageDialog.InputException exception) {
                MessageDialog.error("All fields are required", "Error");
            }
            catch (SelectDiagramException exception) {
                MessageDialog.error("A diagram has to be selected", "Error");
            }
            catch (SQLException | DatabaseHandler.LoginException ignored) {
            }
        }

        private void deleteAction()
        {
            try {
                DiagramView diagramItem = getSelectedDiagram();
                diagramItem.diagramModel.delete();
            }
            catch (SelectDiagramException exception) {
                MessageDialog.error("A diagram has to be selected", "Error");
            }
            catch (SQLException | DatabaseHandler.LoginException ignored) {
            }
        }
    }

    public static class JOptionPaneMultiInput
    {
        private static final String TITLE = "Please fill the following fields";
        private static final short MAX_CHAR_IN_TEXT_FIELD = 20;
        public String title = "";
        public String name = "";
        public String email = "";

        public JOptionPaneMultiInput(String title, String name, String email) throws MessageDialog.InputException
        {
            this.title = title;
            this.name = name;
            this.email = email;
            getInput();
        }

        public JOptionPaneMultiInput() throws MessageDialog.InputException
        {
            getInput();
        }

        private void getInput() throws MessageDialog.InputException {
            JPanel panel = new JPanel();

            GridLayout gridLayout = new GridLayout(3, 2, 20, 20);
            panel.setLayout(gridLayout);

            JTextField fieldTitle = new JTextField(MAX_CHAR_IN_TEXT_FIELD);
            JTextField fieldName = new JTextField(MAX_CHAR_IN_TEXT_FIELD);
            JTextField fieldEmail = new JTextField(MAX_CHAR_IN_TEXT_FIELD);

            fieldTitle.setText(title);
            fieldName.setText(name);
            fieldEmail.setText(email);

            JLabel labelTitle = new JLabel("Title *");
            JLabel labelName = new JLabel("Your Name *");
            JLabel labelEmail = new JLabel("Your Email");

            panel.add(labelTitle);
            panel.add(fieldTitle);
            panel.add(labelName);
            panel.add(fieldName);
            panel.add(labelEmail);
            panel.add(fieldEmail);

            int result = JOptionPane.showConfirmDialog(null, panel, TITLE, JOptionPane.DEFAULT_OPTION);

            if (result == JOptionPane.OK_OPTION) {
                title = fieldTitle.getText();
                name = fieldName.getText();
                email = fieldEmail.getText();

                if (title.isEmpty() || name.isEmpty()) {
                    throw new MessageDialog.InputException("InputException: Empty input from user");
                }
            }
        }
    }

    public static class SelectDiagramException extends Exception
    {
        public SelectDiagramException(String string)
        {
            super(string);
        }
    }
}
