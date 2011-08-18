/* Flags that can be set on a signature element. */
#define SIG_ELEM_BIND_CAPTURE        1
#define SIG_ELEM_BIND_PRIVATE_ATTR   2
#define SIG_ELEM_BIND_PUBLIC_ATTR    4
#define SIG_ELEM_BIND_ATTRIBUTIVE    (SIG_ELEM_BIND_PRIVATE_ATTR | SIG_ELEM_BIND_PUBLIC_ATTR)
#define SIG_ELEM_SLURPY_POS          8
#define SIG_ELEM_SLURPY_NAMED        16
#define SIG_ELEM_SLURPY_LOL          32
#define SIG_ELEM_SLURPY              (SIG_ELEM_SLURPY_POS | SIG_ELEM_SLURPY_NAMED | SIG_ELEM_SLURPY_LOL)
#define SIG_ELEM_INVOCANT            64
#define SIG_ELEM_MULTI_INVOCANT      128
#define SIG_ELEM_IS_RW               256
#define SIG_ELEM_IS_COPY             512
#define SIG_ELEM_IS_PARCEL           1024
#define SIG_ELEM_IS_OPTIONAL         2048
#define SIG_ELEM_ARRAY_SIGIL         4096
#define SIG_ELEM_HASH_SIGIL          8192
#define SIG_ELEM_DEFAULT_FROM_OUTER  16384
#define SIG_ELEM_IS_CAPTURE          32768
#define SIG_ELEM_UNDEFINED_ONLY      65536
#define SIG_ELEM_DEFINED_ONLY        131072
#define SIG_ELEM_DEFINEDNES_CHECK    (SIG_ELEM_UNDEFINED_ONLY | SIG_ELEM_DEFINED_ONLY)
#define SIG_ELEM_METHOD_SLURPY_NAMED 262144
#define SIG_ELEM_NOMINAL_GENERIC     524288

/* This is how a parameter looks on the inside. Actually, this is a C struct
 * that should match the computed object layout by P6opaque for the type
 * Parameter. So if changing that, this needs to be changed here. */
typedef struct {
    PMC    *st;                   /* S-table, though we don't care about that here. */
    PMC    *sc;                   /* Serialization context, though we don't care about that here. */
    PMC    *spill;                /* Attribute spill storage. */
    STRING *variable_name;        /* The name in the lexpad to bind to, if any. */
    PMC    *named_names;          /* List of the name(s) that a named parameter has. */
    PMC    *type_captures;        /* Name(s) that we bind the type of a parameter to. */
    INTVAL flags;                 /* Various flags about the parameter. */
    PMC    *nominal_type;         /* The nominal type of the parameter. */
    PMC    *post_constraints;     /* Array of any extra constraints; we will do a
                                   * smart-match against each of them. For now, we
                                   * always expect an array of blocks. */
    PMC    *coerce_type;          /* The type to coerce the value to, if any. */
    STRING *coerce_method;        /* Name of the method to call to coerce; for X we do $val.X. */
    PMC    *sub_llsig;            /* Any nested signature. */
    PMC    *default_closure;      /* The default value closure. */
    PMC    *container_descriptor; /* Descriptor for the container we bind into, if any. */
    PMC    *attr_package;         /* Package part of an attributive binding. */
} Rakudo_Parameter;

/* This is how a signature looks on the inside. Actually, this is a C struct
 * that should match the computed object layout by P6opaque for the type
 * Signature. So if changing that, this needs to be changed here. */
typedef struct {
    PMC    *st;                 /* S-table, though we don't care about that here. */
    PMC    *sc;                 /* Serialization context, though we don't care about that here. */
    PMC    *spill;              /* Attribute spill storage. */
    PMC    *params;             /* Array of objects that are all parameters. */
    PMC    *rtype;              /* Return type. */
} Rakudo_Signature;

/* Flags we can set on the Context PMC.
 *
 * ALREADY_CHECKED indicates that we have determined that all of the arguments
 * can be bound to positional parameters without any further type checking
 * (because the multi-dispatch cache told us so) and any named parameters are
 * automatically going into the named slurpy variable.
 *
 * ALREADY_BOUND indicates that the variables have already been bound into the
 * lexpad and means the bind_signature op is thus a no-op. This happens if we
 * had to do a bindability check in the multi-dispatch anyway.
 */
#define PObj_P6S_ALREADY_CHECKED_FLAG   PObj_private0_FLAG
#define PObj_P6S_ALREADY_BOUND_FLAG     PObj_private1_FLAG

/* Gets the ID of a 6model object PMC. */
INTVAL Rakudo_smo_id(void);

/* Functions we want to share to provide the interface to the binder. */
INTVAL Rakudo_binding_bind(PARROT_INTERP, PMC *lexpad, PMC *sig_pmc,
                    PMC *capture, INTVAL no_nom_type_check,
                    STRING **error);
void Rakudo_binder_set_top_type(PMC *type);
PMC * Rakudo_binder_get_top_type(void);
void Rakudo_binder_set_junction_type(PMC *type);
PMC * Rakudo_binder_get_junction_type(void);
/* for perl6.ops */
PMC * Rakudo_binding_parcel_from_rpa(PARROT_INTERP, PMC *rpa, PMC *fill);
PMC * Rakudo_binding_iter_from_rpa(PARROT_INTERP, PMC *rpa, PMC *list);
PMC * Rakudo_binding_list_from_rpa(PARROT_INTERP, PMC *rpa, PMC *type, PMC *flat);

/* Things Rakudo_binding_bind_llsig may return to indicate a problem. */
#define BIND_RESULT_OK       0
#define BIND_RESULT_FAIL     1
#define BIND_RESULT_JUNCTION 2
