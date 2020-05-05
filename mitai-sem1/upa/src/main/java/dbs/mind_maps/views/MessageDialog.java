package dbs.mind_maps.views;

import javax.swing.*;


public class MessageDialog
{
    public static void info(String title, String message)
    {
        JOptionPane.showMessageDialog(null, message, title, JOptionPane.INFORMATION_MESSAGE);
    }

    public static void error(String title, String message)
    {
        JOptionPane.showMessageDialog(null, message, title, JOptionPane.ERROR_MESSAGE);
    }

    public static class SingleInput
    {
        private static final short MAX_CHAR_IN_TEXT_FIELD = 20;

        public static String get(String title, String label, String defaultValue)
        {
            JTextField fieldInput = new JTextField(MAX_CHAR_IN_TEXT_FIELD);
            fieldInput.setText(defaultValue);
            JPanel panel = new JPanel();
            JLabel labelInput = new JLabel(label);

            panel.add(labelInput);
            panel.add(Box.createHorizontalStrut(10));  // space
            panel.add(fieldInput);

            int result = JOptionPane.showConfirmDialog(null, panel, title, JOptionPane.DEFAULT_OPTION);

            if (result == JOptionPane.OK_OPTION) {
                return fieldInput.getText();
            }
            else {
                return defaultValue;
            }
        }
    }

    public static class InputException extends Exception
    {
        public InputException(String string)
        {
            super(string);
        }
    }
}
