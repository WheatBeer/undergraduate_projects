#include <stdio.h>
#include "project3.h"

extern int TraceLevel;
extern float clocktime;

struct distance_table {
  int costs[MAX_NODES][MAX_NODES];
};
struct distance_table dt0;
struct NeighborCosts   *neighbor0;

/* students to write the following two routines, and maybe some others */

struct RoutePacket packet; //set packet as local variable

void printdt0( int MyNodeNumber, struct NeighborCosts *neighbor, struct distance_table *dtptr );

void rtinit0() {
    int i, j;
    struct distance_table *thisnode;

    thisnode = &dt0; //set struct pointer
    printf("****************************************************************\n");
    printf("At time t = %f, rtinit0() called\n", clocktime);

    // initiate minimum cost and distance table as INFINITY
    for (i = 0; i < 4; i++) {
        packet.mincost[i] = INFINITY;
        for (j = 0; j < 4; j++) {
            thisnode->costs[i][j]= INFINITY;
        }
    }

    neighbor0 = getNeighborCosts(0); //get neighborcosts from project3.c

    // set neighborcosts in distance table
    for (i = 0; i < 4; i++) {
        thisnode->costs[i][i] = neighbor0->NodeCosts[i];
    }

    // check each distance from node i to node j in comparison to the packet
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            if (packet.mincost[i] > thisnode->costs[i][j]) {
                packet.mincost[i] = thisnode->costs[i][j];
            }
        }
    }

    packet.sourceid = 0; //set packet's sourceID
    for (i = 1; i < MAX_NODES; i++) { //start from 1 bc this node# is 0
            packet.destid = i;
            toLayer2(packet);  //use tolayer2 to send to another node
            printf("At time t = %f, node %d sends packet to node %d with: %d %d %d %d\n", clocktime, packet.sourceid, i , packet.mincost[0], packet.mincost[1], packet.mincost[2], packet.mincost[3]);
        }
    printdt0(0, neighbor0, thisnode);
    printf("****************************************************************\n");
    printf("\n\n");
}


void rtupdate0( struct RoutePacket *rcvdpkt ) {
    int i, j, src, update = 0;
    struct distance_table *thisnode;

    thisnode = &dt0; 

    for (i = 0; i < 4; i++) {
            packet.mincost[i] = INFINITY;
    }
    printf("****************************************************************\n");
    printf("At time t = %f, rtupdate0() called\n", clocktime);

    // check received source ID
    src = rcvdpkt->sourceid;
    printf("Received packet from node %d\n", src);

    for (i = 0; i < 4; i++) {
        // if there is new mincost change, update distance table.
        if (thisnode->costs[i][src] > (thisnode->costs[src][src] + rcvdpkt->mincost[i])) {
            thisnode->costs[i][src] = (thisnode->costs[src][src] + rcvdpkt->mincost[i]);
            update = 1;
        }
    }

    // If we update distance table above, change this node's packet mincost
    if (update == 1) {
        for (i = 0; i < 4; i++) {
            for (j = 0; j < 4; j++) {
                if (packet.mincost[i] > thisnode->costs[i][j]) {
                    packet.mincost[i] = thisnode->costs[i][j];
                }
            }
        }

        packet.sourceid = 0; //set packet's sourceID
        for (i = 1; i < MAX_NODES; i++) { //start from 1 bc this node# is 0
            packet.destid = i; 
            toLayer2(packet); //use tolayer2 to send to another node
            printf("At time t = %f, node %d sends packet to node %d with: %d %d %d %d\n", clocktime, packet.sourceid, i , packet.mincost[0], packet.mincost[1], packet.mincost[2], packet.mincost[3]);
        }
    }
    printdt0(0, neighbor0, thisnode);
    printf("****************************************************************\n");
    printf("\n\n");
}


/////////////////////////////////////////////////////////////////////
//  printdt
//  This routine is being supplied to you.  It is the same code in
//  each node and is tailored based on the input arguments.
//  Required arguments:
//  MyNodeNumber:  This routine assumes that you know your node
//                 number and supply it when making this call.
//  struct NeighborCosts *neighbor:  A pointer to the structure 
//                 that's supplied via a call to getNeighborCosts().
//                 It tells this print routine the configuration
//                 of nodes surrounding the node we're working on.
//  struct distance_table *dtptr: This is the running record of the
//                 current costs as seen by this node.  It is 
//                 constantly updated as the node gets new
//                 messages from other nodes.
/////////////////////////////////////////////////////////////////////
void printdt0( int MyNodeNumber, struct NeighborCosts *neighbor, 
		struct distance_table *dtptr ) {
    int       i, j;
    int       TotalNodes = neighbor->NodesInNetwork;     // Total nodes in network
    int       NumberOfNeighbors = 0;                     // How many neighbors
    int       Neighbors[MAX_NODES];                      // Who are the neighbors

    // Determine our neighbors 
    for ( i = 0; i < TotalNodes; i++ )  {
        if (( neighbor->NodeCosts[i] != INFINITY ) && i != MyNodeNumber )  {
            Neighbors[NumberOfNeighbors] = i;
            NumberOfNeighbors++;
        }
    }
    // Print the header
    printf("                via     \n");
    printf("   D%d |", MyNodeNumber );
    for ( i = 0; i < NumberOfNeighbors; i++ )
        printf("     %d", Neighbors[i]);
    printf("\n");
    printf("  ----|-------------------------------\n");

    // For each node, print the cost by travelling thru each of our neighbors
    for ( i = 0; i < TotalNodes; i++ )   {
        if ( i != MyNodeNumber )  {
            printf("dest %d|", i );
            for ( j = 0; j < NumberOfNeighbors; j++ )  {
                    printf( "  %4d", dtptr->costs[i][Neighbors[j]] );
            }
            printf("\n");
        }
    }
    printf("\n");
}    // End of printdt0

