#include "controllerdaemon.h"
#include <iostream>
#include "DB_Operation.h"

using namespace std;

//Constructor: Arguments are references to Request Queue and the Empty Checking Semaphore respectively
ControllerDaemon::ControllerDaemon(QQueue <QString> *reqQ, QSemaphore *sem)
{
    cout<<"ControllerDaemon Started\n";
    reqQueue = reqQ;
    semEmpty=sem;
    db=getMySqlDB();
    if(db.isOpenError())
    {
        cout<<"Error: "<<db.lastError().text().toAscii().data();
    }
}

//The Main Daemon Process, which controls ROOT, handles output and updates database
void ControllerDaemon::run()
{
    QString reqID;

    while(1)
    {
        //Blocking wait on Queue if it is empty
        semEmpty->acquire();

        //Pop the front element of queue
        cout<<"Popped\n";
        reqID=reqQueue->front();
        reqQueue->pop_front();

        //TODO: Remove this line
        cout<<endl<<reqID.toAscii().data()<<endl;

        root = new QProcess();                         //New Process
        connect(root,SIGNAL(readyReadStandardOutput()),this,SLOT(stdoutReader()));  //for reading stdout

        //Read all the actual parameters
        QString actual_params_query="select seq_no,value from actual_parameter,parameters"
                " where actual_parameter.parameter_id=parameters.parameter_id and"
                " output_id='"+reqID+"'"
                " order by seq_no";
        QSqlQuery *actual_params_result = new QSqlQuery(db);

        if(!actual_params_result->exec(actual_params_query))    //execute the query and check if it was unsuccessful
        {
            cout<<"Error Occured while getting actual parameters !!\n"<<actual_params_result->lastError().text().toAscii().data()<<endl;
            cout<<"Query: "<<actual_params_query.toAscii().data()<<endl;
            continue;
        }

        actual_params_result->first();
        QStringList params;
        while(actual_params_result->isValid())
        {
            cout<<"Seq_no: "<<actual_params_result->value(0).toInt()<<" Value: "<<actual_params_result->value(1).toByteArray().data()<<endl;
            params.push_back(actual_params_result->value(1).toString());
            actual_params_result->next();
        }

        //TODO: Do it with Configuration, i.e. QSettings
        //QString ROOT_path("/home/swap/root/bin/root");

        //Get input script
        QString exe_file_query="select exefile,op_type from template "
                "where template_id=(select template_id from output where output_id='"+reqID+"')";


        QSqlQuery *exe_file_result = new QSqlQuery(db);

        if(!exe_file_result->exec(exe_file_query))
        {
            cout<<"Error Occured while getting Executable !!\n"<<exe_file_result->lastError().text().toAscii().data()<<endl;
            cout<<"Query: "<<exe_file_query.toAscii().data()<<endl;
            continue;
        }
\
        //get input script filename
        exe_file_result->first();
        QString exe_filename(exe_file_result->value(0).toString());

        /*
         * TODO: Get script base directory from settings
         */
        QString exe_dir="/home/akshay/work/codes/faltu/root/";
        //QString exe_dir="/home/tifr-viit/Apex/";
        cout<<"Executing\n";

        //Create File
        /*
         * TODO: Find op_file_path from Settings
         */
        QString op_file_path="/var/www/Apex/output/dynamic/"+reqID+"/";
        if(!(new QDir("/"))->mkpath(op_file_path)){
            cout<<"Error Occured while creating Output directory..!!";
            continue;
        }

        op_file = new QFile(op_file_path+"op.txt");
        op_file->open(QFile::ReadWrite);

        cout<<"Executing: "<<QString(exe_dir+exe_filename).toAscii().data()<<endl;

        root->setWorkingDirectory(op_file_path);
        root->start(exe_dir+exe_filename,params);
//        root->start(ROOT_path,QStringList() << "-l"<<"-b"<<script_dir+script_filename);

        /*
         * TODO: Print pid only in debug mode
         */
        cout<<"pid is: "<<root->pid()<<endl;

        /*
         * TODO: Set a waiting limit of few minutes.
         * Discuss with Mohanty Sir and Thaware Sir
         * If not completed kill the task and write timeout in DB.
         */
        root->waitForReadyRead();
        root->write(".q\n");
        root->waitForBytesWritten();
        root->waitForReadyRead();
        root->terminate();
        root->waitForFinished();

        /*
         * TODO: Handle Timed out requests
         */
        cout<<"Exit code is: "<<root->exitCode()<<endl;
        QString op_status="READY";
        if(root->exitCode()!=0){
            op_status="FAILED";
        }

        delete root;
        delete op_file;
        QString query;

        query="update output set op_state='"+op_status+"' where output_id='"+reqID+"'";
        QSqlQuery *update_op = new QSqlQuery(db);
        if(!update_op->exec(query))
        {
            cout<<"Error Occurred while updating database."<<update_op->lastError().text().data()<<endl;
            cout<<"Query ="<<query.toAscii().data()<<endl;
            continue;
        }
        if(op_status=="READY")
            cout<<"Successfully Completed Request\n";
        else
            cout<<"Request Failed\n";
    }
}
void ControllerDaemon::terminate()
{
    int x;
}

/* Slot connected to QProcess Object which executes and handles ROOT.
 * Writes everything to the Output file, whose path is stored in op_file
 */
void ControllerDaemon::stdoutReader()
{
    QString stdout(root->readAll());
    cout<<stdout.toAscii().data();

    op_file->write(stdout.toAscii());
}
