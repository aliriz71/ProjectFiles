package sqlconnectionspack;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;

public class A02_219539881 { 

    private static final String URL = "jdbc:mysql://127.0.0.1:3306/a02";
    private static final String USER = "<user>";
    private static final String PASSWORD = "<password>";
    private static final String CURRENT_TERM = "S2024";

    public static void main(String[] args) 
    {
        try (Connection connection = DriverManager.getConnection(URL, USER, PASSWORD);
             Statement statement = connection.createStatement()) 
        {

            checkUnexpectedGrades(statement);
            checkEnrollmentAmbiguities(statement);
            checkEnrollmentYearExceptions(statement);
            checkNameAmbiguities(statement);

        }catch (SQLException e) 
        {
            e.printStackTrace();
        }
    }

    private static void checkUnexpectedGrades(Statement statement) throws SQLException 
    {
        String query = "SELECT COUNT(*) FROM enrollment WHERE term = '" + CURRENT_TERM + "' AND grade IS NOT NULL";
        ResultSet rs = statement.executeQuery(query);
        if (rs.next() && rs.getInt(1) > 0) 
        {
            System.out.println("Unexpected grade for current term.");
        }
    }

    private static void checkEnrollmentAmbiguities(Statement statement) throws SQLException 
    {
        String query = "SELECT snum, cnum, COUNT(*) FROM enrollment GROUP BY snum, cnum HAVING COUNT(*) > 1";
        ResultSet rs = statement.executeQuery(query);
        if (rs.next()) 
        {
            System.out.println("Enrollment ambiguity.");
        }
    }

    private static void checkEnrollmentYearExceptions(Statement statement) throws SQLException 
    {
        String query = "SELECT student.snum, enrollment.cnum " +
                       "FROM student " +
                       "JOIN enrollment ON student.snum = enrollment.snum " +
                       "WHERE CAST(SUBSTR(enrollment.cnum, 3, 1) AS UNSIGNED) > student.year " +
                       "ORDER BY student.snum, enrollment.cnum";
        ResultSet rs = statement.executeQuery(query);
        boolean hasExceptions = false;
        while (rs.next()) 
        {
            if (!hasExceptions) 
            {
                System.out.println("Enrollment year exception(s):");
                hasExceptions = true;
            }
            System.out.printf("%-9s %-5s%n", rs.getInt("snum"), rs.getString("cnum"));
        }
    }

    private static void checkNameAmbiguities(Statement statement) throws SQLException 
    {
        String query = "SELECT name FROM (" +
                       "SELECT pname AS name FROM professor " +
                       "UNION ALL " +
                       "SELECT sname AS name FROM student" +
                       ") names GROUP BY name HAVING COUNT(*) > 1";
        ResultSet rs = statement.executeQuery(query);
        if (rs.next()) 
        {
            System.out.println("Name ambiguity.");
        }
    }
}
