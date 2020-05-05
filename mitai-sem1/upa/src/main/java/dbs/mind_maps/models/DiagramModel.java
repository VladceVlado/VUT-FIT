package dbs.mind_maps.models;

import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.List;
import java.util.LinkedList;
import java.util.Objects;


public class DiagramModel
{
    private static final String DATE_PATTERN = "MM/dd/yyyy HH:mm:ss";

    public Integer id;
    public String title;
    public String name;
    public String email;
    public String date;

    public DiagramModel()
    {
    }

    public void update(String title, String name, String email) throws SQLException, DatabaseHandler.LoginException
    {
        String emailNotNull = Objects.requireNonNullElse(email, "");

        final String QUERY = "UPDATE diagrams SET " +
                "name = '" + title + "', " +
                "author_name = '" + name + "', " +
                "author_email = '" + emailNotNull + "', " +
                "last_modified = current_timestamp " +
                "WHERE id = " + id;

        PreparedStatement statement = DatabaseHandler.getInstance().getConnection().prepareStatement(QUERY);
        statement.executeQuery();
    }

    public void delete() throws SQLException, DatabaseHandler.LoginException
    {
        final String QUERY1 = "DELETE FROM nodes WHERE diagram_id = " + id;
        final String QUERY2 = "DELETE FROM diagrams WHERE id = " + id;

        PreparedStatement statement1 = DatabaseHandler.getInstance().getConnection().prepareStatement(QUERY1);
        statement1.executeQuery();

        PreparedStatement statement2 = DatabaseHandler.getInstance().getConnection().prepareStatement(QUERY2);
        statement2.executeQuery();
    }

    public static void insert(String title, String name, String email) throws SQLException, DatabaseHandler.LoginException
    {
        final String QUERY = "INSERT INTO diagrams " +
                "(name, author_name, author_email, last_modified) " +
                "VALUES ('" + title + "', '" + name + "', '" + email + "', current_timestamp)";

        PreparedStatement statement = DatabaseHandler.getInstance().getConnection().prepareStatement(QUERY);
        statement.executeQuery();
    }

    public static List<DiagramModel> select() throws SQLException, DatabaseHandler.LoginException
    {
        final String QUERY = "SELECT id, name, author_name, author_email, last_modified FROM diagrams";

        PreparedStatement statement = DatabaseHandler.getInstance().getConnection().prepareStatement(QUERY);
        ResultSet resultSet = statement.executeQuery();

        List<DiagramModel> diagrams = new LinkedList<>();

        while (resultSet.next()) {
            DiagramModel diagram = new DiagramModel();

            diagram.id = resultSet.getInt(1);
            diagram.title = resultSet.getString(2);
            diagram.name = resultSet.getString(3);
            diagram.email = resultSet.getString(4);
            DateFormat dateFormat = new SimpleDateFormat(DATE_PATTERN);
            diagram.date = dateFormat.format(resultSet.getDate(5));

            diagrams.add(diagram);
        }

        return diagrams;
    }
}
