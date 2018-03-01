#ifndef SEIMS_MPI_H
#define SEIMS_MPI_H

#include "seims.h"
#include "utilities.h"
#include "invoke.h"
#include "ModelMain.h"

#ifdef USE_MONGODB
#include "DataCenterMongoDB.h"

#endif /* USE_MONGODB */
#include "clsReach.h"

#include "mpi.h"
#include "mongoc.h"
#include "gdal.h"
#include "ogrsf_frmts.h"

#include <map>
#include <set>
#include <vector>
#include <string>

#define WORK_TAG 0
#define MASTER_RANK 0
#define SLAVE0_RANK 1 ///< Rank of this slave processor in SlaveGroup is 0
#define MAX_UPSTREAM 4
#define MSG_LEN 5
#define MCW MPI_COMM_WORLD

using namespace std;
/*!
 * \brief Simple struct of subbasin information for task allocation
 * \TODO Should this SubbasinStruct be integrated into Subbasin class defined in clsSubbasin of data module? by LJ
 */
struct SubbasinStruct {
    SubbasinStruct(int sid, int gidx) : id(sid), group(gidx),
                                        updown_order(-1), downup_order(-1), calculated(false),
                                        qOutlet(0.f),
                                        downStream(nullptr) {
        upStreams.clear();
    }
    int id; ///< Subbasin ID, start from 1
    int group; ///< Group index, start from 0 to (group number - 1)

    int updown_order; ///< up-down stream order
    int downup_order; ///< down-up stream order
    bool calculated; ///< whether this subbasin is already calculated

    /****** Parameters need to transferred among subbasins *******/
    float qOutlet; ///< flow out the subbasin outlet

    SubbasinStruct *downStream; ///< down stream subbasin \sa SubbasinStruct
    vector<SubbasinStruct *> upStreams; ///< up stream subbasins
};

/*!
 * \brief Read reach table from MongoDB and create reach topology for task allocation.
 */
int CreateReachTopology(MongoClient *client, string &dbname, string &group_method, int group_size,
                        map<int, SubbasinStruct *> &subbasins, set<int> &group_set);
/*!
 * \brief
 * \param subbasinMap
 * \param groupSet
 * \param input_args
 * \return
 */
int MasterProcess(map<int, SubbasinStruct *> &subbasinMap, set<int> &groupSet, InputArgs *input_args);

/*!
 * \brief
 * \param world_rank
 * \param numprocs
 * \param nSlaves
 * \param slaveComm
 * \param input_args
 */
void CalculateProcess(int world_rank, int numprocs, int nSlaves, MPI_Comm slaveComm, InputArgs *input_args);

#endif /* SEIMS_MPI_H */
