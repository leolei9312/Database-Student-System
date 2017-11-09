//main.cpp
//database
//EECS495 - Intro to DB
//Programming Assignment (3)
//Date: 15-Nov-2016
//Team Memebers: Menglei Lei & Norah Altuwaim

//include required Libraries
#include <stdio.h>
#include <mysql.h> 
#include <time.h>
#include <iostream> 
#include <string.h>
#include <string>

//include the namespace std for cout cin
using namespace std;

//SQL Conecction
MYSQL *conn = mysql_init (NULL);

//declare globale variables
string stdId;
string stdPass;
int temp;
int month;
string quarter;
string year;
string year2;
string quarter2;

//declare functions
void login();           //this function is for student to login
void Transcript();      //this views the student's transcripts with previous and current courses taken
void CD();              //CD is Transcript sub-function to show the detail of a given course.
void Enroll();          //this function to let student enroll to some courses
void Withdraw();        //this function to let student withdraw from courses he is currently taking
void PD();              //this function is to view the Personal Details
void menu();            //this function is to firstly display all the options a student can choose from

//main part of the program
int main (int argc, const char * argv[])
{
    string strmain;
   
    
    //open connection
    mysql_real_connect(conn, "localhost", "root", "", "project3-nudb", 0, NULL, CLIENT_MULTI_RESULTS);
    
    //frame to login first then navigate through the menu options
    while(true)
    {
        login();
            while(true)
            {
                menu();
                //get the student option and store it in strmain
                cout << "Next Operation:";
                cin >> strmain;
                
                if(strmain == "1")
                {
                    Transcript();
                    continue;
                }
                else if(strmain == "2")
                {
                    Enroll();
                    continue;
                }
                else if(strmain == "3")
                {
                    Withdraw();
                    continue;
                }
                else if(strmain == "4")
                {
                    PD();
                    continue;
                }
                else if(strmain == "5")
                {
                    break;
                }
                else
                {
                    cout << "Error: Unknow Operation.";
                    continue;
                }
            }
    }
    
    //close connection
    mysql_close(conn);
    return 0;
}

//***************************************************************************************************//
//The functions for the students menu options

//LOGIN FUNCTION
void login()
{
    //declare local varaibles
    MYSQL_RES *res_set;
    MYSQL_ROW row;
    string querylo;
    int num;
    
    //frame of login
    while(true)
    {
        //interface
        cout << "*************************************************\n";
        cout << "          Welcome To Student System.\n";
        cout << "*************************************************\n";
        cout << "     Made by Menglei Lei and Norah Altuwaim.\n";
        //Prompt the user to give his/her id and passwords and store them in stdId and stdPass
        cout << "Enter your Student Id:\n";
        cin >> stdId;
        cout << "Enter your Password:\n";
        cin >> stdPass;
        
        //Querey to get the student with id stdId Password from the DB
        querylo = "SELECT Password FROM student where Id = '"+stdId+"';";
        mysql_query(conn,querylo.c_str());
        res_set = mysql_store_result(conn);
        num = (int)mysql_num_rows(res_set);
        
        //if there is no rows returned from the query, then the stdId (username) does not exist
        if(num == 0)
        {
            cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n    Username doesn't exist.\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n";
            continue;
        }
        
        //if the there is a row returend from the query, then stdId (username) exist, so check the password
        else
        {
            row = mysql_fetch_row(res_set);
            if(stdPass == row[0])
            {
                cout << "$$$$$$$$$$$$$$$$$$$$$$$$$\n Login Successfully.\n$$$$$$$$$$$$$$$$$$$$$$$$\n";
                break;
            }
            else
            {
                cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n        Password incorrect.\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n";
                continue;
            }
        }
    } //end of while
    
    //free resources
    mysql_free_result(res_set);
}


//MENU FUNCTION
void menu()
{
    
    //clock system
    time_t timeSec = time(NULL);
    struct tm *timeinfo = localtime(&timeSec);
    
    //interface
    cout << "                        Tab\n";
    cout << "***************************************************************\n";
    cout << "1.Transcript**2.Enroll**3.Withdraw**4.Personal Detail**5.logout\n";
    cout << "***************************************************************\n";
    
    //working on the time
    temp = timeinfo -> tm_year + 1900;
    month = timeinfo -> tm_mon + 1;
    year = std::to_string(temp);
    
    //transfert month to quarter
    if(month >= 9 && month <= 12)
    {
        quarter = "Q1";
    }
    else if(month >= 1 && month <= 3)
    {
        quarter = "Q2";
    }
    else if(month >= 4 && month <= 6)
    {
        quarter = "Q3";
    }
    else
    {
        quarter = "Q4";
    }
    
    cout << "ID: " << stdId << " Quarter " << quarter << " YEAR: " << year << "\n";
    
    //query command to get the courses in his/her transcript
    MYSQL_RES *res_set;
    MYSQL_ROW row;
    string queryme = "SELECT UoSCode FROM transcript where StudId = '"+stdId+"' and Semester = '"+quarter+"' and Year = '"+year+"' and Grade is null;";
    mysql_query(conn,queryme.c_str());
    res_set = mysql_store_result(conn);
    
    //display the courses
    if(res_set == NULL)
    {
        cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n   Student has no course.\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n";
    }
    else
    {
        int numrows = (int)mysql_num_rows(res_set);
        
        cout << "Courses:\n";
        
        for(int i = 0; i < numrows; i ++)
        {
            row = mysql_fetch_row(res_set);
                if(row != NULL)
                {
                    cout << row[0] <<"\n";
                }
        }
    }
    //free resources
    mysql_free_result(res_set);
}


//TRANSCRIPT FUNCTION
void Transcript()
{
    //declare local variables
    string strtr;
    
    //frame
    while(true)
    {
        cout << "                        Tab\n";
        cout << "*****************************************************\n";
        cout << "**********Course Detail*************Go Back**********\n";
        cout << "*****************************************************\n";
        
        //query command
        MYSQL_RES *res_set;
        MYSQL_ROW row;
        string query = "SELECT UoSCode, Grade FROM transcript where StudId = '"+stdId+"';";
        mysql_query(conn,query.c_str());
        res_set = mysql_store_result(conn);
        
        //if the result of the query is NULL, then inform the user that he/she had not taken a course yet
        if(res_set == NULL)
        {
            cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n Student has no course currently.\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n";
        }
        //otherwise disply the course number and the Grade
        else
        {
            cout << "Courses \t Grade\n";
            int numrows = (int)mysql_num_rows(res_set);
            for(int i = 0; i < numrows; i ++)
            {
                row = mysql_fetch_row(res_set);
                if(row != NULL)
                {
                    if(row[1] != NULL)
                    {
                        cout << row[0] << "\t" << row[1] << "\n";
                    }
                    else
                    {
                        cout << row[0] << "\t" << "NULL" << "\n";
                    }
                }//end of if
                
            }//end of For
        }//end of else
        
        //free resource
        mysql_free_result(res_set);
        
        //get the user option
        cout << "Course detail input 1. Go back to menu input 2\n";
        cin >> strtr;
        
        //if option is 2 go back to menu
        if(strtr == "2")
        {
            return;
        }
        
        //if option is 1 then disply the course details with the help of function CD()
        else if(strtr == "1")
        {
            CD(); // go to course details function
            continue;
        }
        else
        {
            cout << "Error: Unknown Operation\n";
            continue;
        }
    }//end of while
}

//COURSE DETAILS FUNCTION
void CD()
{
    //declare local variables
    string back;
    string strcd;
    
    //get the student course number he/she needs details about
    cout << "Input course number.\n";
    cin >> strcd;
    
    //query to get all the details of the course from the DB
    MYSQL_RES *res_set;
    MYSQL_ROW row;
    string querycd = "select a.UoSCode, a.UoSName, a.Year, a.Semester, a.Enrollment, a.MaxEnrollment, a.Name, b.Grade from( select UoSCode, UoSName, Year, Semester, Enrollment, MaxEnrollment, Name from (unitofstudy inner join uosoffering using(UoSCode)) inner join faculty on Id = InstructorId where UoSCode = ( select UoSCode from transcript where StudId = '"+stdId+"' and UoSCode = '"+strcd+"') and Semester = (select Semester from transcript where StudId = '"+stdId+"' and UoSCode = '"+strcd+"') and Year = (select Year from transcript where StudId = "+stdId+" and UoSCode = '"+strcd+"')) a, transcript as b where b.UoSCode = '"+strcd+"' and b.StudId = '"+stdId+"';";
    
    mysql_query(conn,querycd.c_str());
    res_set = mysql_store_result(conn);
    int Numrows = (int)mysql_num_rows(res_set);
    
    //display the result of the query
    
    //if there is not result from the query, this mean that the user didn't take this course yet
    if(Numrows == 0)
    {
        cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n"<< "Student has not taken this course.\n" << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n";
    }
    
    //otherwise display the details
    else
    {
        int numrows = (int)mysql_num_rows(res_set);
        for(int i = 0; i < numrows; i ++)
        {
            row = mysql_fetch_row(res_set);
            if(row != NULL)
            {
                if(row[7] != NULL)
                {
                    cout << "CCode:"<<row[0]<<" CName:"<<row[1]<<" Year:"<<row[2]<<" Semester:"<<row[3]<<" Enrollment:"<<row[4]<<" MaxEnrollment:"<<row[5]<<" TeacherName:"<<row[6]<<" Grade:"<<row[7]<<"\n";
                }
                else
                {
                    cout << "CCode:"<<row[0]<<" CName:"<<row[1]<<" Year:"<<row[2]<<" Semester:"<<row[3]<<" Enrollment:"<<row[4]<<" MaxEnrollment:"<<row[5]<<" TeacherName:"<<row[6]<<" Grade: NULL"<<"\n";
                }
            }
        }//end of For
    }//end of else
    
    //free resources
    mysql_free_result(res_set);
    
    //press to return to transcript
    while(true)
    {
        cout << "Press 1 to go back to transcript.\n";
        cin >> back;
        
        if(back == "1")
        {
            return;
        }
        else
        {
            cout << "Error: Unknown Operation.\n";
        }
    }//end of while
}

//ENROLL FUNCTION
void Enroll()
{
    //declare local varibales
    string queryen;
    string stren;
    string q2, y2;
    int numrows, Numrows;
    time_t timeSec = time(NULL);
    MYSQL_RES *res_set;
    MYSQL_ROW row;
    
    //working on Time
    struct tm *timeinfo = localtime(&timeSec);
    temp = timeinfo -> tm_year + 1900;
    month = timeinfo -> tm_mon + 1;
    year = std::to_string(temp);
    year2 = std::to_string(temp + 1);
    
    //display the courses that the user can take. taking into considration current qauarter and next quarter courses only
    //with taking care of his/her prefernces of maximum enrolment and type of class
    cout << "The courses that can be Enrolled:\n";
    
    //transfert month to quarter
    if(month >= 9 && month <= 12)
    {
        quarter = "Q1";
        quarter2 = "Q2";
    }
    else if(month >= 1 && month <= 3)
    {
        quarter = "Q2";
        quarter2 = "Q3";
    }
    else if(month >= 4 && month <= 6)
    {
        quarter = "Q3";
        quarter2 = "Q4";
    }
    else
    {
        quarter = "Q4";
        quarter2 = "Q1";
    }
    
    //display the courses
    while(true)
    {
        //query to get his/her prefrences
        queryen = "select MaximumPreferredEnrollment, NonPreferredClassroomType from student where Id = '"+stdId+"';";
        mysql_query(conn,queryen.c_str());
        res_set = mysql_store_result(conn);
        row = mysql_fetch_row(res_set);
        
        if(quarter == "Q1") //for Q1 courses
        {
            if(row[0] == NULL && row[1] == NULL)//when the student doesn't have any prefrences
            {
                //display everything
                queryen = "select distinct UoSCode, MaxEnrollment, UoSName, ClassroomId, Semester, Year, Type from(select distinct UoSCode, MaxEnrollment, UoSName, ClassroomId, Semester, Year from( select distinct UoSCode, MaxEnrollment, UoSName, Semester, Year from uosoffering inner join unitofstudy using(UoSCode) where (Semester = 'Q1' and Year = '"+year+"') or (Semester = '"+quarter2+"' and Year = '"+year2+"')) table1 inner join lecture using(UoSCode, Year, Semester)) table2 inner join classroom using(ClassroomId) where (MaxEnrollment is not null) and (Type is not null);";
            }
            else if(row[0] != NULL && row[1] == NULL)//when the student have MaximumPreferredEnrollment but NOT NonPreferredClassroomType
            {
                queryen = "select distinct UoSCode, MaxEnrollment, UoSName, ClassroomId, Semester, Year, Type from(select distinct UoSCode, MaxEnrollment, UoSName, ClassroomId, Semester, Year from( select distinct UoSCode, MaxEnrollment, UoSName, Semester, Year from uosoffering inner join unitofstudy using(UoSCode) where (Semester = 'Q1' and Year = '"+year+"') or (Semester = '"+quarter2+"' and Year = '"+year2+"')) table1 inner join lecture using(UoSCode, Year, Semester)) table2 inner join classroom using(ClassroomId) where (MaxEnrollment <= (select MaximumPreferredEnrollment from student where Id = '"+stdId+"') or MaxEnrollment is not null) and (Type is not null);";
            
            }
            else if(row[0] == NULL && row[1] != NULL)//when the student doesn't have MaximumPreferredEnrollment but have NonPreferredClassroomType
            {
                queryen = "select distinct UoSCode, MaxEnrollment, UoSName, ClassroomId, Semester, Year, Type from(select distinct UoSCode, MaxEnrollment, UoSName, ClassroomId, Semester, Year from( select distinct UoSCode, MaxEnrollment, UoSName, Semester, Year from uosoffering inner join unitofstudy using(UoSCode) where (Semester = 'Q1' and Year = '"+year+"') or (Semester = '"+quarter2+"' and Year = '"+year2+"')) table1 inner join lecture using(UoSCode, Year, Semester)) table2 inner join classroom using(ClassroomId) where (MaxEnrollment is not null) and (Type != (select NonPreferredClassroomType from student where Id = '"+stdId+"'));";
            }
            else //when the student have both MaximumPreferredEnrollment and NonPreferredClassroomType
            {
            queryen = "select distinct UoSCode, MaxEnrollment, UoSName, ClassroomId, Semester, Year, Type from(select distinct UoSCode, MaxEnrollment, UoSName, ClassroomId, Semester, Year from( select distinct UoSCode, MaxEnrollment, UoSName, Semester, Year from uosoffering inner join unitofstudy using(UoSCode) where (Semester = 'Q1' and Year = '"+year+"') or (Semester = '"+quarter2+"' and Year = '"+year2+"')) table1 inner join lecture using(UoSCode, Year, Semester)) table2 inner join classroom using(ClassroomId) where (MaxEnrollment <= (select MaximumPreferredEnrollment from student where Id = '"+stdId+"')) and (Type != (select NonPreferredClassroomType from student where Id = '"+stdId+"'));";
            }
        } //end of if
        
        else //for Q2 courses
        {
            if(row[0] == NULL && row[1] == NULL)//when the student doesn't have any prefrences
            {
                //display everything
                queryen = "select distinct UoSCode, MaxEnrollment, UoSName, ClassroomId, Semester, Year, Type from(select distinct UoSCode, MaxEnrollment, UoSName, ClassroomId, Semester, Year from( select distinct UoSCode, MaxEnrollment, UoSName, Semester, Year from uosoffering inner join unitofstudy using(UoSCode) where (Semester = '"+quarter+"' and Year = '"+year+"') or (Semester = '"+quarter2+"' and Year = '"+year+"')) table1 inner join lecture using(UoSCode, Year, Semester)) table2 inner join classroom using(ClassroomId) where (MaxEnrollment is not null) and (Type is not null);";
            }
            else if(row[0] != NULL && row[1] == NULL)//when the student have MaximumPreferredEnrollment but NOT NonPreferredClassroomType
            {
                queryen = "select distinct UoSCode, MaxEnrollment, UoSName, ClassroomId, Semester, Year, Type from(select distinct UoSCode, MaxEnrollment, UoSName, ClassroomId, Semester, Year from( select distinct UoSCode, MaxEnrollment, UoSName, Semester, Year from uosoffering inner join unitofstudy using(UoSCode) where (Semester = '"+quarter+"' and Year = '"+year+"') or (Semester = '"+quarter2+"' and Year = '"+year+"')) table1 inner join lecture using(UoSCode, Year, Semester)) table2 inner join classroom using(ClassroomId) where (MaxEnrollment <= (select MaximumPreferredEnrollment from student where Id = '"+stdId+"') or MaxEnrollment is not null) and (Type is not null);";
            }
            else if(row[0] == NULL && row[1] != NULL)//when the student doesn't have MaximumPreferredEnrollment but have NonPreferredClassroomType
            {
                queryen = "select distinct UoSCode, MaxEnrollment, UoSName, ClassroomId, Semester, Year, Type from(select distinct UoSCode, MaxEnrollment, UoSName, ClassroomId, Semester, Year from( select distinct UoSCode, MaxEnrollment, UoSName, Semester, Year from uosoffering inner join unitofstudy using(UoSCode) where (Semester = '"+quarter+"' and Year = '"+year+"') or (Semester = '"+quarter2+"' and Year = '"+year+"')) table1 inner join lecture using(UoSCode, Year, Semester)) table2 inner join classroom using(ClassroomId) where (MaxEnrollment is not null) and (Type != (select NonPreferredClassroomType from student where Id = '"+stdId+"'));";
            }
            else //when the student have both MaximumPreferredEnrollment and NonPreferredClassroomType
            {
                queryen = "select distinct UoSCode, MaxEnrollment, UoSName, ClassroomId, Semester, Year, Type from(select distinct UoSCode, MaxEnrollment, UoSName, ClassroomId, Semester, Year from( select distinct UoSCode, MaxEnrollment, UoSName, Semester, Year from uosoffering inner join unitofstudy using(UoSCode) where (Semester = '"+quarter+"' and Year = '"+year+"') or (Semester = '"+quarter2+"' and Year = '"+year+"')) table1 inner join lecture using(UoSCode, Year, Semester)) table2 inner join classroom using(ClassroomId) where (MaxEnrollment <= (select MaximumPreferredEnrollment from student where Id = '"+stdId+"')) and (Type != (select NonPreferredClassroomType from student where Id = '"+stdId+"'));";
           }
    }//end of else
      
    //display the result of the query
    mysql_query(conn,queryen.c_str());
    res_set = mysql_store_result(conn);
    numrows = (int)mysql_num_rows(res_set);
    for(int i = 0; i < numrows; i ++)
    {
        row = mysql_fetch_row(res_set);
        if(row != NULL)
        {
                cout << row[0] << "\t" << row[2] << "\n";
        }
    }//end of for
        
    //free resourses
    mysql_free_result(res_set);
        
    //get the number of the course the student wants to enroll in
    cout << "Please enter the course number you want to enroll.\n";
    cin >> stren;
     
    //when the quarter is Q1 then check if the student can enroll to this course
    if(quarter == "Q1"){
        queryen = "select UoSCode, Semester, Year from (select * from lecture inner join unitofstudy using(UoSCode) where (Semester = 'Q1' and Year = '"+year+"') or (Semester = '"+quarter2+"' and Year = '"+year2+"')) table1 where UoSCode = '"+stren+"';";
    }
    else //when the quarter is Q2 then check if the student can enroll to this course
    {
        queryen = "select UoSCode, Semester, Year from (select * from lecture inner join unitofstudy using(UoSCode) where (Semester = '"+quarter+"' and Year = '"+year+"') or (Semester = '"+quarter2+"' and Year = '"+year+"')) table1 where UoSCode = '"+stren+"';";
    }
    mysql_query(conn,queryen.c_str());
    res_set = mysql_store_result(conn);
    Numrows = (int)mysql_num_rows(res_set);
     
     //if there is no result from the query, then tell the student that he/she can't be enrolled in this course
    if(Numrows == 0)
    {
        cout << "The course can not be enrolled.\n";
        //free resources
        mysql_free_result(res_set);
        continue;
    }
    else if(Numrows == 1) //otherwise when the query return a result tuple call Enroll stored procedure
    {
        //call Enroll stored procedure
        row = mysql_fetch_row(res_set);
        queryen = "call Enroll('"+stren+"', '"+stdId+"', '"+row[1]+"', '"+row[2]+"', @output);";
        
        if(mysql_query(conn,queryen.c_str()) == 0)//to handel the returened value of the ENROLL procedure
        {
            do
            {
                if(mysql_field_count(conn) > 0)
                {
                    res_set = mysql_store_result(conn);
                    //free resources
                    mysql_free_result(res_set);
                }
            
            } while(mysql_next_result(conn) == 0);
        }
        row = mysql_fetch_row(res_set);
        
        cout << row[0] << "\n";
        
        //to disply the prerequist courses that student has not staisfied yet
        if(strcmp(row[0], "Error: The course can not be taken because prerequisite courses.") == 0)
        {
            //query to get the prerequsit courses
            queryen = "select PrereqUoSCode from requires where UoSCode = '"+stren+"' and PrereqUoSCode not in (select UoSCode from transcript where StudId = '"+stdId+"' and Grade != 'D' and Grade is not null);";
            
            mysql_query(conn,queryen.c_str());
            res_set = mysql_store_result(conn);
            int numb = (int)mysql_num_rows(res_set);
            
            //display the result of the query
            cout << "The following prerequisite course is not met:\n";
            for(int i = 0; i < numb; i ++)
            {
                row = mysql_fetch_row(res_set);
                if(row != NULL)
                {
                cout << row[0] << "\n";
            }
          }
        }
        
        //otherwise enroll student in the course
        else if(strcmp(row[0], "Successfully enrolled.") == 0)
        {
            //query to display the the warning from the trigger
            queryen = "select @message;";
            mysql_query(conn,queryen.c_str());
            res_set = mysql_store_result(conn);
            row = mysql_fetch_row(res_set);
            
            //when the enrollment is less than half
            if(strcmp(row[0], "The enrollment is smaller than half.") == 0)
            {
                cout << "Warning: The enrollment is smaller than half.\n";
                queryen = "set @message = '';";
                mysql_query(conn,queryen.c_str());
                res_set = mysql_store_result(conn);
            }
        }
        
        //get the student option and store in stren
        cout << "Press 1 to return to menu. Press 2 to continue Enrolling.";
        cin >> stren;
        
        //if option is 1 go back to main menu
        if(stren == "1")
        {
            return;
        }
        
        //if option is 2 continue to enroll again to enroll in another course
        else if(stren == "2")
        {
            continue;
        }
    }//end of else if
        
    else //if the result of the query is > 1 then ask the user to specify the qaurter and year
    {
        cout << "This course is provided in two quarters. Please specify the quarter and Year.\n";
        cout << "The year and quarters is under below:\n";
        
        //disply the quarter and year this course is provided in
        for(int i = 0; i < Numrows; i ++)
        {
            row = mysql_fetch_row(res_set);
            cout << "1. Quarter:" << row[1] << "Year:" << row[2] <<"\n";
        }
        cout << "Quarter:  \n";
        cin >> q2;
        cout << "  Year:  \n";
        cin >> y2;
        
        //call the ENROLL stored procedure
        row = mysql_fetch_row(res_set);
        queryen = "call Enroll('"+stren+"', '"+stdId+"', '"+q2+"', '"+y2+"', @output);";
        
        
        if(mysql_query(conn,queryen.c_str()) == 0) //to handel the returened value of the ENROLL procedure
        {
            do
            {
                if(mysql_field_count(conn) > 0)
                {
                    res_set = mysql_store_result(conn);
                    //free resources
                    mysql_free_result(res_set);
                }
            }while(mysql_next_result(conn) == 0);
        }
        row = mysql_fetch_row(res_set);
        
        cout << row[0] << "\n";
        
        if(strcmp(row[0], "Error: The course can not be taken because prerequisite courses.") == 0)
        {
            queryen = "select PrereqUoSCode from requires where UoSCode = '"+stren+"' and PrereqUoSCode not in (select UoSCode from transcript where StudId = '"+stdId+"' and Grade != 'D' and Grade is not null);";
            
            mysql_query(conn,queryen.c_str());
            res_set = mysql_store_result(conn);
            int numb = (int)mysql_num_rows(res_set);
            
            //display the exact prerequist courses that were not met by the student
            cout << "The following prerequisite course is not met:\n";
            for(int i = 0; i < numb; i ++)
            {
                row = mysql_fetch_row(res_set);
                if(row != NULL)
                {
                    cout << row[0] << "\n";
                }
            }
        }
        
        //otherwise enroll student in the course
        else if(strcmp(row[0], "Successfully enrolled.") == 0)
        {
            queryen = "select @message;";
            mysql_query(conn,queryen.c_str());
            res_set = mysql_store_result(conn);
            row = mysql_fetch_row(res_set);
            
            //query to display the the warning from the trigger
            if(strcmp(row[0], "The enrollment is smaller than half.") == 0)
            {
                //when the enrollment is less than half
                cout << "Warning: The enrollment is smaller than half.\n";
                queryen = "set @message = '';";
                mysql_query(conn,queryen.c_str());
                res_set = mysql_store_result(conn);
            }
        }
        //get the student option and store in stren
        cout << "Press 1 to return to menu. Press 2 to continue Enrolling.";
        cin >> stren;
        
        //if option is 1 go back to main menu
        if(stren == "1")
        {
            return;
        }
        //if option is 2 continue to enroll again to enroll in another course
        else if(stren == "2")
        {
            continue;
        }
    }//end of else if
    }//end of while
}


//WITHDRAW FUNCTION
void Withdraw()
{
    //decalre variables
    string Wcourse;
    string option;
    string query;
    
    //Display the courses that the student can withdraw from (courses that he is currently taking or registered for next quarter, that is grade is NULL)
    cout << "These are the courses that you can withdraw from:\n";
    //Submit query to display the courses that a student can be withdraw from
    MYSQL_RES *res_set;
    MYSQL_ROW row;
    //the query
    query = "SELECT transcript.UoSCode, UoSName, Semester, Year FROM transcript join unitofstudy WHERE transcript.UoSCode = unitofstudy.UoSCode AND StudId = '"+stdId+"' AND transcript.Grade is NULL;";
    
    //disply the results
    mysql_query(conn, query.c_str());
    res_set = mysql_store_result(conn);
    int numrows = (int)mysql_num_rows(res_set);
    
    //if the query does not return any tuple, then the student has not any course to withdraw from
    if ( numrows == 0 )
    {
        cout << "There are no courses to withdraw from\n";
    }
    
    //otherwise disply the courses the student can withdraw from
    else
    {
        // Display results
        for (int i = 0; i < numrows; i++)
        {
            row = mysql_fetch_row( res_set );
            if( row != NULL )
            {
                cout << "Course Code: " << row[0] << endl;
                cout << "Course Name: " << row[1] << endl;
                cout << "Semester : " << row[2] << endl;
                cout << "Year: " << row[3] << endl;
            }
        }
    }
    while (true)
    {
        //Get the the withdraw course name from the user and store it in Wcourse
        cout << "Enter the course you want to withdraw from:\n";
        cin >> Wcourse;
        
        // Submit query to get the courses that a student can be withdraw from
        MYSQL_RES *res_set;
        MYSQL_ROW row;
        query = "SELECT transcript.UoSCode FROM transcript WHERE StudId = '"+stdId+"' AND transcript.Grade is NULL and UoSCode = '"+Wcourse+"';";
        
        mysql_query(conn, query.c_str());
        res_set = mysql_store_result(conn);
        int numrows = (int)mysql_num_rows(res_set);
        row = mysql_fetch_row(res_set);
        
            if(numrows == 0)
            {
                cout << "Can not withdraw from this course.\n";
                cout << "Press 1 to go back to menu. Press other to continue withdrawing.";
                cin >> option;
                if(option == "1") return;
                else continue;
            }
            else
            {
                //remove Wcourse from transcript AND
                //decrease Enrollment by 1 in UoSoffering
                //by calling Stored Procedure withdraw
                query = "call withdraw ('"+Wcourse+"', '"+stdId+"')";
                
                if(mysql_query(conn,query.c_str()) == 0)//to handle the stored prosedure WITHDRAW
                {
                    do
                    {
                        if(mysql_field_count(conn) > 0)
                        {
                            res_set = mysql_store_result(conn);
                            mysql_free_result(res_set);
                        }
                    }while(mysql_next_result(conn) == 0);
                }
                query = "select @message;";
                mysql_query(conn,query.c_str());
                res_set = mysql_store_result(conn);
                row = mysql_fetch_row(res_set);
                if(strcmp(row[0], "rollback") != 0){
                cout << " You successfully withdraw from course!!" << "\n";
                query = "select @message;";
                mysql_query(conn,query.c_str());
                res_set = mysql_store_result(conn);
                row = mysql_fetch_row(res_set);
                
                //to disply the warning from the trigger
                if(strcmp(row[0], "The enrollment is smaller than half.") == 0)
                {
                    cout << "Warning: The enrollment is smaller than half.\n";
                    query = "set @message = '';";
                    mysql_query(conn,query.c_str());
                    res_set = mysql_store_result(conn);
                }
                }
                else {
                    cout << "Error occurs. Rollback.\n";
                }
            }//end of else
        
        //get the student option and store in option
        cout << " Enter 1 to continue withdrawing. Enter 2 Go back to main menu.\n";
        cin >> option;
        
        //if option is 1 , then continue withdrawing other course
        if(option == "1")
        {
            continue;
        }
        //if option is 2, then go back to main menu
        else if (option == "2")
        {
            return;
        }
    }//end of while
}


//PERSONAL DETAILS FUNCTON
void PD()
{
    //delcare variable
    string option;
    string maxPrefferedEnrol;
    string classRmType;
    
    //query to display the personal info and prefrences
    string query;
    MYSQL_RES *res_set;
    MYSQL_ROW row;
    while (true)
    {
    query = "SELECT Id, Name, MaximumPreferredEnrollment, NonPreferredClassroomType FROM student where Id = '"+stdId+"';";
    mysql_query(conn, query.c_str());
    res_set = mysql_store_result(conn);
    row = mysql_fetch_row(res_set);
    if(row[3] == NULL) row[3] = "NULL";
    if(row[2] == NULL) row[2] = "NULL";
    
        cout << "ID\t\t\tName\t\tMaximum Preffered Enrollment\t\tNon Preferred classroom Type\n";
        cout << row[0] << "\t\t" << row[1] << "\t\t\t\t" << row[2] << "\t\t\t\t\t\t\t\t\t" << row[3] << "\n";
    
        cout << "\nEnter 1 to Update your prefrences. Enter 2 to go back main menu.\n";
        cin >> option;
    
        //when student choose option 1 to update the prefrences
        if (option == "1")
        {
            //get the maximum prefered Enrollment
            cout << "Enter your Maximum Preffered Enrollment\n";
            cin >> maxPrefferedEnrol;
            if(maxPrefferedEnrol != "null"){
            query = "update student set MaximumPreferredEnrollment = '"+maxPrefferedEnrol+"' where Id = '"+stdId+"';";
            mysql_query(conn, query.c_str());
            res_set = mysql_store_result(conn);
            }
            else {
                query = "update student set MaximumPreferredEnrollment = NULL where Id = '"+stdId+"';";
                mysql_query(conn, query.c_str());
                res_set = mysql_store_result(conn);
            }
            //get the Non Preferred Classroom Type
            cout << "Enter your Non Preferred Classroom Type (e.g tiered, flat, sloping)\n";
            cin >> classRmType;
            
            //if the Non Preferred Classroom Type  is not tiered and not flat and not sloping,
            //then it is not a valid entry, prompt the student to enter again
            if(classRmType != "tiered" && classRmType != "flat" && classRmType != "sloping" &&  classRmType != "null")
            {
                cout << "Your entry is not valid please enter a valid class type (e.g tiered, flat, sloping)!\n";
                continue;
            }
            //if the Non Preferred Classroom Type is either tiered or flat or sloping , then it is a valid entry, update the value in the DB
            else if(classRmType != "null")
            {
                query = "update student set NonPreferredClassroomType = '"+classRmType+"' where Id = '"+stdId+"';";
                mysql_query(conn, query.c_str());
                res_set = mysql_store_result(conn);
            }
            else{
                query = "update student set NonPreferredClassroomType = NULL where Id = '"+stdId+"';";
                mysql_query(conn, query.c_str());
                res_set = mysql_store_result(conn);
            }
        }//end of if
        
        //when the student choose option 2, then go to main menu
        if(option == "2")
        {
            return;
        }
    }    //end of while
    
}














