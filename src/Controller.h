#ifndef CONTROLLER_H
#define	CONTROLLER_H

#include <map>
#include <string>
#include <vector>

class ServiceReader;
class ServiceProcessor;
class TaskProcessor;

typedef std::pair<int,int> procParamP_t; //first: jobID to return when the job is finished; second: Duration in seconds
typedef std::pair<int, procParamP_t> procIDParamP_t; //first: Index of Processor; Second Params to start the job
typedef std::vector<procIDParamP_t> procsToStartV_t;
typedef std::vector<std::string> processorV_t; //List of Processor Type Names to start
typedef std::map<int, ServiceProcessor *> ServiceProcessorIDM_t; /// first: ID of the job; second: serviceProcessor where the ServiceRequest Runs on
typedef std::map<int, TaskProcessor *> TaskProcessorIDM_t; /// first: ID of the job; second: serviceProcessor where the ServiceRequest Runs on
typedef std::vector<int> JobsToKillV_t; ///List of JobIDs to kill

class Controller {
public:
	/**
	 * Reads the ini files, and might end the program if there's an error
	 *
	 * @Param filename Name including path of the System.ini
	 */
	Controller(std::string filename);
    
	/**
	 * Reads the ini files from a standard location, and might end the program if there's an error
	 *
	 * @Param filename Name including path of the System.ini
	 */
	Controller();
    
    ~Controller();

    Controller(const Controller &toCopy);

    
	/**
	 * Returns an list of the type of Processors needed. For each Listmember there needs to be
	 * started an according Taskprocessor in Rose/RT.
	 * The Index here needs to be remembered because the TaskProcessers will be Accessed by this index.
	 *
	 * @Param ProcessorList OUT List of names of processors to start, list is not sorted by type
	 * @Return Number of members of this list; -1 if an error occurred
	 */
	int getServiceProcessors(processorV_t ** processorList);

	/**
	 * Returns a List of procs with params to start
	 * @Param procsToStart OUT: List of procs to start; Undefined if we need to wait or all Jobs are done
	 * @Return: Number of Procs to start; -1 If all Jobs are finished; 0 if we need to wait for jobs to finish
	 */
	int getNextJobs(procsToStartV_t ** procsToStart);

	/**
	 * Tells the Controller that a single Taskprocessor has finished the job with the given ID
	 * @Param id ID of the job that has finished
	 * @Return true if everything's allright; False if an error occurred
	 */
	int jobFinished(int jobID);
    
    /**
     * Tells the Controller that a single TaksProcessor has started the job with the given ID
     * @Param id ID of the job that has finished
	 * @Return true if everything's allright; False if an error occurred
     */
    int jobStarted(int jobID);

	/**
	 * To Call if something is went wrong on while trying to start the job with the given ID
	 * shouldn't happen so I don't know jet how to implement this or what it will return
	 * maybe we terminate everything then
	 */
	int jobStartFailure(int jobID);

	/**
	 * To Call if an Processor throws an exception (takes longer than expected). I don't think we should react to this but it was said to implement
	 */
	int jobException(int jobID);

	/**
	 * To Call if an processor Terminates Unexpected
	 * @param jobID Job that terminated unexpected
     * @return list of jobIDs that needs to be stopped because of that (confirm stoped jobs with jobAbortConfirmation(int jobID))
	 */
	JobsToKillV_t jobUnexpectedTerminated(int jobID);
    
    /**
     * Tells the Controller that a single TaksProcessor has started the job with the given ID
     * @Param id ID of the job that has been aborted
	 * @Return true if everything's allright; False if an error occurred
     */
    int jobAbortConfirmation(int jobID);
    
    /**
     * @returns the current timestamp
     */
    std::string getTimestamp();
    
    
private:
    ServiceReader * serviceReader; /// ServiceReader of the System.ini
    int nextServiceRequestID; ///ID of the next Job to start
    TaskProcessorIDM_t taskProcIDMap; ///Map to identifiy the ServiceRequests that have finished;
    
    /**
     * Links the JobIDs with the TaskProcessor in the TaskProcIDMap; increases the nextServiceRequestID
     * @param procsToStart Has the jobIDs to link
     * @param taskProc TaskProcessor to link the jobID to
     */
    void writeJobMap(procsToStartV_t * procsToStart, TaskProcessor *taskProc);
    
};

#endif /* CONTROLLER_H */
