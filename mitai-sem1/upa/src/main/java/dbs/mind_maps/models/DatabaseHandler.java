package dbs.mind_maps.models;

import oracle.jdbc.pool.OracleDataSource;

import java.io.*;
import java.sql.Connection;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.Optional;

import static java.util.Optional.empty;
import static java.util.Optional.of;


public class DatabaseHandler
{
    private static Optional<DatabaseHandler> handlerInstance = empty();
    private Connection connection;

    private DatabaseHandler(String url, String username, String password) throws LoginException
    {
        try {
            OracleDataSource ods = new OracleDataSource();
            ods.setURL(url);
            ods.setUser(username);
            ods.setPassword(password);
            connection = ods.getConnection();
        }
        catch (SQLException exception) {
            throw new LoginException("LoginException:  " + exception.getMessage());
        }
    }

    public static DatabaseHandler getInstance() throws LoginException
    {
        if (DatabaseHandler.handlerInstance.isEmpty()) {
            throw new LoginException("LoginException: database handler was not initialized before usage");
        }

        return DatabaseHandler.handlerInstance.get();
    }

    public static void login(String url, String username, String password) throws LoginException
    {
        DatabaseHandler dbh = new DatabaseHandler(url, username, password);
        DatabaseHandler.handlerInstance = of(dbh);
    }

    public static void close() throws SQLException
    {
        if (DatabaseHandler.handlerInstance.isPresent()) {
            DatabaseHandler.handlerInstance.get().connection.close();
            DatabaseHandler.handlerInstance = empty();
        }
    }

    public Connection getConnection()
    {
        return connection;
    }

    public static class LoginException extends Exception
    {
        public LoginException(String string)
        {
            super(string);
        }
    }

    public void runSqlScript(String sqlFile, String delimiter) throws IOException, SQLException
    {
        String s;
        StringBuilder sb = new StringBuilder();

        String sqlFilePath = System.getProperty("user.dir") + "/db/" + sqlFile;

        FileReader fr = new FileReader(new File(sqlFilePath));
        BufferedReader br = new BufferedReader(fr);

        while ((s = br.readLine()) != null) {
            sb.append(s);
        }
        br.close();

        String[] statements = sb.toString().split(delimiter);

        boolean autoCommitOriginal = connection.getAutoCommit();
        connection.setAutoCommit(false);
        Statement st = connection.createStatement();

        for (String statement : statements) {
            try {
                st.executeUpdate(statement);
            }
            catch (SQLException ignored) {
            }
        }
        connection.commit();
        connection.setAutoCommit(autoCommitOriginal);
    }

    public void dropDb() throws IOException
    {
        try {
            runSqlScript("db-drop.sql",";");
        }
        catch (SQLException ignored) {
        }
    }

    public void createDb() throws IOException
    {
        try {
            runSqlScript("db-create-tables.sql",";");
            runSqlScript("db-create-others.sql","/");
        }
        catch (SQLException ignored) {
        }
    }

    public void fillDb() throws IOException
    {
        try {
            // create records using SQL
            runSqlScript("db-fill-data.sql",";");
        }
        catch (SQLException ignored) {
        }
    }
}
