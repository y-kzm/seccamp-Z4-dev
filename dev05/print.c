/*
 * print.c
 * 2021/09/06 ~
 * Yokoo Kazuma
*/

#include <stdio.h>
#include "param.h"
#include "bgp.h"
#include "includes.h"

/*
    >>>>    PRINT BGP MSG.    <<<<
*/

/*---------- print_hdr ----------*/
void print_hdr(struct bgp_hdr *bh)
{
    int i;

    fprintf(stdout, "Marker: ");
    for(i=0; i<MARKER_LEN; i++){
        fprintf(stdout, " %x", bh->marker[i]);
    } fprintf(stdout, " \n");
    fprintf(stdout, "Len:  %u\n", ntohs(bh->len));
    fprintf(stdout, "Type: %u\n\n", bh->type);  
}

/*---------- print_open ----------*/
void print_open(struct bgp_open_opt *bop)
{
    int i;

    fprintf(stdout, "Marker: ");
    for(i=0; i<MARKER_LEN; i++){
        fprintf(stdout, " %x", bop->hdr.marker[i]);
    } fprintf(stdout, " \n");
    fprintf(stdout, "Len:  %u\n", ntohs(bop->hdr.len));
    fprintf(stdout, "Type: %u\n", bop->hdr.type);  

    fprintf(stdout, "Version:  %u\n", bop->version);
    fprintf(stdout, "MyAS:     %u\n", ntohs(bop->myas));
    fprintf(stdout, "HoldTime: %u\n", ntohs(bop->holdtime));
    fprintf(stdout, "Id:       %s\n", inet_ntoa(bop->id));
    fprintf(stdout, "Opt_Len:  %u\n\n", bop->opt_len);
}

/*---------- print_update ----------*/
void print_update(struct bgp_update *bu, struct withdrawn_routes *wr, struct total_path_attrib *tpa)
{
    int i;

    /* Header. */
    fprintf(stdout, "Marker: ");
    for(i=0; i<MARKER_LEN; i++){
        fprintf(stdout, " %x", bu->hdr.marker[i]);
    } fprintf(stdout, " \n");
    fprintf(stdout, "Len:  %u\n", ntohs(bu->hdr.len));
    fprintf(stdout, "Type: %u\n", bu->hdr.type); 

    /* Update msg. */
    fprintf(stdout, "Withdrawn Routes Len: %u\n", ntohs(wr->len));
    // もし到達不能経路があるならprint処理
    fprintf(stdout, "Total Path Len      : %u\n", ntohs(tpa->total_len)); 
    
    /* Path Attrib. */
    fprintf(stdout, "Path Attributes.\n"); 
    

    printf("\n");
}

/*
    >>>>    PRINT PATH ATTRIB..    <<<<
*/
/*---------- print_origin ----------*/
void print_origin(struct pa_origin *origin)
{
    fprintf(stdout, "  Path Attrib: ORIGIN\n");
    fprintf(stdout, "    flags : 0x%x\n", origin->flags);
    fprintf(stdout, "    code  : %x\n", origin->code);
    fprintf(stdout, "    len   : %x\n", origin->len);
    fprintf(stdout, "    origin: %x\n", origin->origin);
}

/*---------- print_as_path ----------*/
// 可変長対応可能に！
void print_as_path(struct pa_as_path *as_path)
{
    fprintf(stdout, "  Path Attrib: AS_PATH\n");
    fprintf(stdout, "    flags : 0x%x\n", as_path->flags);
    fprintf(stdout, "    code  : %x\n", as_path->code);
    fprintf(stdout, "    len   : %x\n", ntohs(as_path->len));
    fprintf(stdout, "    AS Path sgmnt.\n");
    fprintf(stdout, "      Segment type: %u\n", as_path->sgmnt.sgmnt_type);
    fprintf(stdout, "      Segment len : %u\n", as_path->sgmnt.sgmnt_len); 
    fprintf(stdout, "      AS2: %u\n", ntohs(as_path->sgmnt.sgmnt_value));
}

/*---------- print_next_hop ----------*/
void print_next_hop(struct pa_next_hop *next_hop)
{
    fprintf(stdout, "  Path Attrib: NEXT_HOP\n");
    fprintf(stdout, "    flags : 0x%x\n", next_hop->flags);
    fprintf(stdout, "    code  : %x\n", next_hop->code);
    fprintf(stdout, "    len   : %x\n", next_hop->len);
    fprintf(stdout, "    Next hop: %s\n", inet_ntoa(next_hop->nexthop));
}

/*---------- print_med ----------*/
void print_med(struct pa_multi_exit_disc *med)
{
    fprintf(stdout, "  Path Attrib: MULTI_EXIT_DISC\n");
    fprintf(stdout, "    flags : 0x%x\n", med->flags);
    fprintf(stdout, "    code  : %x\n", med->code);
    fprintf(stdout, "    len   : %x\n", med->len);
    fprintf(stdout, "    med   : %x\n", ntohl(med->med));
}

/*---------- print_nlri ----------*/
// 改良の余地あり...
void print_nlri(struct nlri_network *networks_raw, struct nlri_network *networks, int k)
{
    int i;

    fprintf(stdout, "  "); 
    for(i = 0; i < IPV4_BLOCKS_NUM; i ++) { 
        networks->prefix[i] = networks_raw->prefix[i];
        if(i > k-1)
            networks->prefix[i] = 0;
        if(i == IPV4_BLOCKS_NUM-1)
            fprintf(stdout, "%u", networks->prefix[i]); 
        else 
            fprintf(stdout, "%u.", networks->prefix[i]);
    }
    fprintf(stdout, "/%u\n", networks->prefix_len); 
}
