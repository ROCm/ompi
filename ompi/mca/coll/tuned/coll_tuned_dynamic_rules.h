
/*
 * Copyright (c) 2004-2005 The Trustees of Indiana University and Indiana
 *                         University Research and Technology
 *                         Corporation.  All rights reserved.
 * Copyright (c) 2004-2005 The University of Tennessee and The University
 *                         of Tennessee Research Foundation.  All rights
 *                         reserved.
 * Copyright (c) 2004-2005 High Performance Computing Center Stuttgart,
 *                         University of Stuttgart.  All rights reserved.
 * Copyright (c) 2004-2005 The Regents of the University of California.
 *                         All rights reserved.
 * Copyright (c) 2011-2012 FUJITSU LIMITED.  All rights reserved.
 * Copyright (c) 2025      Amazon.com, Inc. or its affiliates.  All rights
 *                         reserved.
 * $COPYRIGHT$
 *
 * Additional copyrights may follow
 *
 * $HEADER$
 */

#ifndef MCA_COLL_TUNED_DYNAMIC_RULES_H_HAS_BEEN_INCLUDED
#define MCA_COLL_TUNED_DYNAMIC_RULES_H_HAS_BEEN_INCLUDED

#include "ompi_config.h"

BEGIN_C_DECLS

enum coll_rules_magic_message_sizes {
    /* our message sizes are unsigned, but our json reader library returns only
    signed integers. Hopefully no one is sending 9 exabytes in a single message. */
    COLL_RULES_MESSAGE_SIZE_INF = SSIZE_MAX,
    COLL_RULES_VALUE_NOT_PROVIDED = -1,
};

enum comm_rank_distro_t {
    COLL_RULES_DISTRO_ANY=0,
    COLL_RULES_DISTRO_DISJOINT,
    COLL_RULES_DISTRO_SINGLENODE,
    COLL_RULES_DISTRO_COUNT, /* keep as last entry, count of valid rules */
};

/* translate the comm_rank_distro_t values from string */
int coll_rules_comm_rank_distro_from_str(const char *name, enum comm_rank_distro_t *distro);
/* translate the comm_rank_distro_t values to string */
const char* coll_rules_comm_rank_distro_to_str(enum comm_rank_distro_t distro);

typedef struct msg_rule_s {
    /* keep track of which collective this rule is for */
    int coll_id;     /* the collective ID for this message rule */

    /* RULE */
    size_t msg_size_min;     /* message size */
    size_t msg_size_max; /* message size max (defaults to SSIZE_MAX)*/

    /* RESULT */
    int result_alg;              /* result algorithm to use */
    int result_topo_faninout;    /* result topology fan in/out to use (if applicable) */
    long result_segsize;         /* result segment size to use */
    int result_max_requests;     /* maximum number of outstanding requests (if applicable) */
} ompi_coll_msg_rule_t;


typedef struct com_rule_s {
    /* paranoid / debug */
    int mpi_comsize_min;  /* which MPI comm size is this for */
    int mpi_comsize_max;
    enum comm_rank_distro_t comm_rank_distribution;

    int coll_id; /* unique alg rule id */

    /* RULE */
    int n_rules;
    ompi_coll_msg_rule_t *msg_rules;

}  ompi_coll_com_rule_t;


typedef struct alg_rule_s {
    /* paranoid / debug */
    int coll_id; /* unique alg rule id */

    /* RULE */
    int n_com_sizes;
    ompi_coll_com_rule_t *com_rules;

} ompi_coll_alg_rule_t;

/* function prototypes */

/* these are used to build the rule tables (by the read file routines) */
ompi_coll_alg_rule_t* ompi_coll_tuned_mk_alg_rules (int n_alg);
ompi_coll_com_rule_t* ompi_coll_tuned_mk_com_rules (int n_com_rules, int coll_id);
ompi_coll_msg_rule_t* ompi_coll_tuned_mk_msg_rules (int n_msg_rules, int coll_id, int mpi_comsize);

/* debugging support */
int ompi_coll_tuned_dump_msg_rule (ompi_coll_msg_rule_t* msg_p);
int ompi_coll_tuned_dump_com_rule (ompi_coll_com_rule_t* com_p);
int ompi_coll_tuned_dump_alg_rule (ompi_coll_alg_rule_t* alg_p);
int ompi_coll_tuned_dump_all_rules (ompi_coll_alg_rule_t* alg_p);

/* free alloced memory routines, used by file and tuned component/module */
int ompi_coll_tuned_free_msg_rules_in_com_rule (ompi_coll_com_rule_t* com_p);
int ompi_coll_tuned_free_coms_in_alg_rule (ompi_coll_alg_rule_t* alg_p);
int ompi_coll_tuned_free_all_rules (ompi_coll_alg_rule_t* alg_p);


/* the IMPORTANT routines, i.e. the ones that do stuff for everyday communicators and collective calls */

ompi_coll_com_rule_t* ompi_coll_tuned_get_com_rule_ptr (ompi_coll_alg_rule_t* rules, int alg_id, struct ompi_communicator_t *comm);

int ompi_coll_tuned_get_target_method_params (ompi_coll_com_rule_t* base_com_rule, size_t mpi_msgsize,
                                              int* result_topo_faninout, int* result_segsize,
                                              int* max_requests);


END_C_DECLS
#endif /* MCA_COLL_TUNED_DYNAMIC_RULES_H_HAS_BEEN_INCLUDED */

