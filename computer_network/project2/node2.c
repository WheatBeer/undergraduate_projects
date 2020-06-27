#include <stdio.h>
#include "project3.h"

extern int TraceLevel;
extern float clocktime;

struct distance_table {
  int costs[MAX_NODES][MAX_NODES];
};
struct distance_table dt2;
struct NeighborCosts   *neighbor2;

/* students to write the following two routines, and maybe some others */

struct RoutePacket packet;

void printdt2( int MyNodeNumber, struct NeighborCosts *neighbor, struct distance_table *dtptr );

void rtinit2() {
    int i, j;
    struct distance_table *thisnode;

    thisnode = &dt2;
    printf("****************************************************************\n");
    printf("At time t = %f, rtinit2() called\n", clocktime);

    for (i = 0; i < 4; i++) {
        packet.mincost[i] = INFINITY;
        for (j = 0; j < 4; j++) {
            thisnode->costs[i][j]= INFINITY;
        }
    }

    neighbor2 = getNeighborCosts(2);

    for (i = 0; i < 4; i++) {
        thisnode->costs[i][i] = neighbor2->NodeCosts[i];
    }

    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            if (packet.mincost[i] > thisnode->costs[i][j]) {
                packet.mincost[i] = thisnode->costs[i][j];
            }
        }
    }

    packet.sourceid = 2;
    for (i = 0; i < MAX_NODES; i++) {
        if (i != packet.sourceid) {
            packet.destid = i;
            toLayer2(packet);
            printf("At time t = %f, node %d sends packet to node %d with: %d %d %d %d\n", clocktime, packet.sourceid, i , packet.mincost[0], packet.mincost[1], packet.mincost[2], packet.mincost[3]);
        }
    }
    printdt2(2, neighbor2, thisnode);
    printf("****************************************************************\n");
    printf("\n\n");
}


void rtupdate2( struct RoutePacket *rcvdpkt ) {
    int i, j, src, update = 0;
    struct distance_table *thisnode;

    thisnode = &dt2;

    for (i = 0; i < 4; i++) {
            packet.mincost[i] = INFINITY;
    }

    printf("****************************************************************\n");
    printf("At time t = %f, rtupdate2() called\n", clocktime);

    src = rcvdpkt->sourceid;
    printf("Received packet from node %d\n", src);

    for (i = 0; i < 4; i++) {
        if (thisnode->costs[i][src] > (thisnode->costs[src][src] + rcvdpkt->mincost[i])) {
            thisnode->costs[i][src] = (thisnode->costs[src][src] + rcvdpkt->mincost[i]);
            update = 1;
        }
    }

    if (update == 1) {
        for (i = 0; i < 4; i++) {
            for (j = 0; j < 4; j++) {
                if (packet.mincost[i] > thisnode->costs[i][j]) {
                    packet.mincost[i] = thisnode->costs[i][j];
                }
            }
        }
        packet.sourceid = 2;
        for (i = 0; i < MAX_NODES; i++) {
            if (i != packet.sourceid) {
                packet.destid = i;
                toLayer2(packet);
                printf("At time t = %f, node %d sends packet to node %d with: %d %d %d %d\n", clocktime, packet.sourceid, i , packet.mincost[0], packet.mincost[1], packet.mincost[2], packet.mincost[3]);
            }
        }
    }

    printdt2(2, neighbor2, thisnode);
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
void printdt2( int MyNodeNumber, struct NeighborCosts *neighbor, 
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
}    // End of printdt2

