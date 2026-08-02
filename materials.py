materials = {

    # ============================================================
    # POLYMERS C-H-O-N-S
    # ============================================================

    # Polypropylene: (C3H6)n
    "PP": {
        "Carbon": 0.856,
        "Hydrogen": 0.144,
        "Oxygen": 0.000,
        "Nitrogen": 0.000,
        "Silicon": 0.000,
        "Other": 0.000,
        "density": 0.905,
    },

    # Polyethylene: (C2H4)n
    "PE": {
        "Carbon": 0.856,
        "Hydrogen": 0.144,
        "Oxygen": 0.000,
        "Nitrogen": 0.000,
        "Silicon": 0.000,
        "Other": 0.000,
        "density": 0.930,
    },

    # High-density polyethylene
    "HDPE": {
        "Carbon": 0.856,
        "Hydrogen": 0.144,
        "Oxygen": 0.000,
        "Nitrogen": 0.000,
        "Silicon": 0.000,
        "Other": 0.000,
        "density": 0.950,
    },

    # Low-density polyethylene
    "LDPE": {
        "Carbon": 0.856,
        "Hydrogen": 0.144,
        "Oxygen": 0.000,
        "Nitrogen": 0.000,
        "Silicon": 0.000,
        "Other": 0.000,
        "density": 0.920,
    },

    # Ultra-high-molecular-weight polyethylene
    "UHMWPE": {
        "Carbon": 0.856,
        "Hydrogen": 0.144,
        "Oxygen": 0.000,
        "Nitrogen": 0.000,
        "Silicon": 0.000,
        "Other": 0.000,
        "density": 0.930,
    },

    # Polystyrene: (C8H8)n
    "PS": {
        "Carbon": 0.923,
        "Hydrogen": 0.077,
        "Oxygen": 0.000,
        "Nitrogen": 0.000,
        "Silicon": 0.000,
        "Other": 0.000,
        "density": 1.050,
    },

    # Polyvinyl chloride: (C2H3Cl)n
    # Chlorine is included in "Other"
    "PVC": {
        "Carbon": 0.385,
        "Hydrogen": 0.048,
        "Oxygen": 0.000,
        "Nitrogen": 0.000,
        "Silicon": 0.000,
        "Other": 0.567,
        "density": 1.400,
    },

    # Polyethylene terephthalate: (C10H8O4)n
    "PET": {
        "Carbon": 0.625,
        "Hydrogen": 0.042,
        "Oxygen": 0.333,
        "Nitrogen": 0.000,
        "Silicon": 0.000,
        "Other": 0.000,
        "density": 1.380,
    },

    # Nylon 6: (C6H11NO)n
    "Nylon 6": {
        "Carbon": 0.632,
        "Hydrogen": 0.097,
        "Oxygen": 0.140,
        "Nitrogen": 0.123,
        "Silicon": 0.000,
        "Other": 0.000,
        "density": 1.130,
    },

    # Nylon 6-6: (C12H22N2O2)n
    "Nylon 6-6": {
        "Carbon": 0.637,
        "Hydrogen": 0.097,
        "Oxygen": 0.142,
        "Nitrogen": 0.124,
        "Silicon": 0.000,
        "Other": 0.000,
        "density": 1.140,
    },

    # Kevlar: aramid polymer, approximately C14H10N2O2
    "Kevlar": {
        "Carbon": 0.691,
        "Hydrogen": 0.041,
        "Oxygen": 0.165,
        "Nitrogen": 0.115,
        "Silicon": 0.000,
        "Other": 0.000,
        "density": 1.440,
    },

    # Nomex: representative aramid polymer
    "Nomex": {
        "Carbon": 0.600,
        "Hydrogen": 0.036,
        "Oxygen": 0.229,
        "Nitrogen": 0.135,
        "Silicon": 0.000,
        "Other": 0.000,
        "density": 1.380,
    },

    # PEEK: polyether ether ketone, C19H12O3
    "PEEK": {
        "Carbon": 0.795,
        "Hydrogen": 0.042,
        "Oxygen": 0.167,
        "Nitrogen": 0.000,
        "Silicon": 0.000,
        "Other": 0.000,
        "density": 1.320,
    },

    # PTFE: (C2F4)n
    # Fluorine is included in "Other"
    "PTFE": {
        "Carbon": 0.240,
        "Hydrogen": 0.000,
        "Oxygen": 0.000,
        "Nitrogen": 0.000,
        "Silicon": 0.000,
        "Other": 0.760,
        "density": 2.200,
    },

    # PVDF: (C2H2F2)n
    "PVDF": {
        "Carbon": 0.375,
        "Hydrogen": 0.031,
        "Oxygen": 0.000,
        "Nitrogen": 0.000,
        "Silicon": 0.000,
        "Other": 0.594,
        "density": 1.780,
    },

    # Polycarbonate: C16H14O3
    "Polycarbonate": {
        "Carbon": 0.755,
        "Hydrogen": 0.055,
        "Oxygen": 0.189,
        "Nitrogen": 0.000,
        "Silicon": 0.000,
        "Other": 0.000,
        "density": 1.200,
    },

    # PMMA: polymethyl methacrylate, C5H8O2
    "PMMA": {
        "Carbon": 0.600,
        "Hydrogen": 0.080,
        "Oxygen": 0.320,
        "Nitrogen": 0.000,
        "Silicon": 0.000,
        "Other": 0.000,
        "density": 1.190,
    },

    # Polyimide, representative Kapton-like composition
    "Polyimide": {
        "Carbon": 0.691,
        "Hydrogen": 0.036,
        "Oxygen": 0.184,
        "Nitrogen": 0.089,
        "Silicon": 0.000,
        "Other": 0.000,
        "density": 1.420,
    },

    # Thermoplastic polyurethane
    # Composition is approximate because TPU is formulation-dependent
    "TPU": {
        "Carbon": 0.600,
        "Hydrogen": 0.070,
        "Oxygen": 0.230,
        "Nitrogen": 0.100,
        "Silicon": 0.000,
        "Other": 0.000,
        "density": 1.200,
    },

    # Polydimethylsiloxane: (C2H6OSi)n
    "PDMS": {
        "Carbon": 0.386,
        "Hydrogen": 0.097,
        "Oxygen": 0.207,
        "Nitrogen": 0.000,
        "Silicon": 0.310,
        "Other": 0.000,
        "density": 0.970,
    },

    # Generic silicone rubber
    "Silicone Rubber": {
        "Carbon": 0.386,
        "Hydrogen": 0.097,
        "Oxygen": 0.207,
        "Nitrogen": 0.000,
        "Silicon": 0.310,
        "Other": 0.000,
        "density": 1.100,
    },

    # Epoxy resin, representative composition
    "Epoxy": {
        "Carbon": 0.750,
        "Hydrogen": 0.065,
        "Oxygen": 0.185,
        "Nitrogen": 0.000,
        "Silicon": 0.000,
        "Other": 0.000,
        "density": 1.200,
    },

    # Phenolic resin, representative composition
    "Phenolic Resin": {
        "Carbon": 0.792,
        "Hydrogen": 0.053,
        "Oxygen": 0.155,
        "Nitrogen": 0.000,
        "Silicon": 0.000,
        "Other": 0.000,
        "density": 1.270,
    },


    # ============================================================
    # COMPOSITES / FIBERS / Glass Based
    # ============================================================

    # Fiberglass: representative E-glass composition
    # Ca, Al, Mg, etc. included in "Other"
    "Fiberglass": {
        "Carbon": 0.000,
        "Hydrogen": 0.000,
        "Oxygen": 0.470,
        "Nitrogen": 0.000,
        "Silicon": 0.530,
        "Other": 0.000,
        "density": 2.550,
    },

    # Carbon fiber: approximately elemental carbon
    "Carbon Fiber": {
        "Carbon": 0.950,
        "Hydrogen": 0.010,
        "Oxygen": 0.020,
        "Nitrogen": 0.020,
        "Silicon": 0.000,
        "Other": 0.000,
        "density": 1.750,
    },

    # Carbon fiber reinforced polymer
    "Carbon Fiber Epoxy": {
        "Carbon": 0.820,
        "Hydrogen": 0.045,
        "Oxygen": 0.120,
        "Nitrogen": 0.015,
        "Silicon": 0.000,
        "Other": 0.000,
        "density": 1.550,
    },

    # Glass fiber reinforced polymer
    "Glass Fiber Epoxy": {
        "Carbon": 0.400,
        "Hydrogen": 0.035,
        "Oxygen": 0.300,
        "Nitrogen": 0.000,
        "Silicon": 0.265,
        "Other": 0.000,
        "density": 1.900,
    },

    # Representative CFRP
    "CFRP": {
        "Carbon": 0.850,
        "Hydrogen": 0.040,
        "Oxygen": 0.100,
        "Nitrogen": 0.010,
        "Silicon": 0.000,
        "Other": 0.000,
        "density": 1.600,
    },

    # Representative GFRP
    "GFRP": {
        "Carbon": 0.400,
        "Hydrogen": 0.035,
        "Oxygen": 0.300,
        "Nitrogen": 0.000,
        "Silicon": 0.265,
        "Other": 0.000,
        "density": 1.900,
    },


    # ============================================================
    # CARBON MATERIALS
    # ============================================================

    "Graphite": {
        "Carbon": 1.000,
        "Hydrogen": 0.000,
        "Oxygen": 0.000,
        "Nitrogen": 0.000,
        "Silicon": 0.000,
        "Other": 0.000,
        "density": 2.267,
    },

    "Graphene": {
        "Carbon": 1.000,
        "Hydrogen": 0.000,
        "Oxygen": 0.000,
        "Nitrogen": 0.000,
        "Silicon": 0.000,
        "Other": 0.000,
        "density": 2.267,
    },

    "Diamond": {
        "Carbon": 1.000,
        "Hydrogen": 0.000,
        "Oxygen": 0.000,
        "Nitrogen": 0.000,
        "Silicon": 0.000,
        "Other": 0.000,
        "density": 3.515,
    },

    "Carbon Nanotubes": {
        "Carbon": 0.980,
        "Hydrogen": 0.005,
        "Oxygen": 0.010,
        "Nitrogen": 0.005,
        "Silicon": 0.000,
        "Other": 0.000,
        "density": 1.400,
    },


    # ============================================================
    # SILICON MATERIALS
    # ============================================================

    "Silicon": {
        "Carbon": 0.000,
        "Hydrogen": 0.000,
        "Oxygen": 0.000,
        "Nitrogen": 0.000,
        "Silicon": 1.000,
        "Other": 0.000,
        "density": 2.330,
    },

    # Silicon carbide: SiC
    "Silicon Carbide": {
        "Carbon": 0.300,
        "Hydrogen": 0.000,
        "Oxygen": 0.000,
        "Nitrogen": 0.000,
        "Silicon": 0.700,
        "Other": 0.000,
        "density": 3.210,
    },

    # Silica: SiO2
    "Silica": {
        "Carbon": 0.000,
        "Hydrogen": 0.000,
        "Oxygen": 0.533,
        "Nitrogen": 0.000,
        "Silicon": 0.467,
        "Other": 0.000,
        "density": 2.200,
    },

    # Generic soda-lime glass
    "Glass": {
        "Carbon": 0.000,
        "Hydrogen": 0.000,
        "Oxygen": 0.470,
        "Nitrogen": 0.000,
        "Silicon": 0.350,
        "Other": 0.180,
        "density": 2.500,
    },

    # Borosilicate glass
    "Borosilicate Glass": {
        "Carbon": 0.000,
        "Hydrogen": 0.000,
        "Oxygen": 0.500,
        "Nitrogen": 0.000,
        "Silicon": 0.370,
        "Other": 0.130,
        "density": 2.230,
    },

    # Aluminosilicate glass
    "Aluminosilicate Glass": {
        "Carbon": 0.000,
        "Hydrogen": 0.000,
        "Oxygen": 0.500,
        "Nitrogen": 0.000,
        "Silicon": 0.300,
        "Other": 0.200,
        "density": 2.600,
    },

    # ============================================================
    # WATER / BIOLOGICAL MATERIALS
    # ============================================================

    # H2O
    "Water": {
        "Carbon": 0.000,
        "Hydrogen": 0.112,
        "Oxygen": 0.888,
        "Nitrogen": 0.000,
        "Silicon": 0.000,
        "Other": 0.000,
        "density": 1.000,
    },

    "Ice": {
        "Carbon": 0.000,
        "Hydrogen": 0.112,
        "Oxygen": 0.888,
        "Nitrogen": 0.000,
        "Silicon": 0.000,
        "Other": 0.000,
        "density": 0.917,
    },

    # Approximate elemental composition of soft tissue
    "Soft Tissue": {
        "Carbon": 0.105,
        "Hydrogen": 0.102,
        "Oxygen": 0.745,
        "Nitrogen": 0.031,
        "Silicon": 0.000,
        "Other": 0.017,
        "density": 1.000,
    },

    # Approximate muscle composition
    "Muscle": {
        "Carbon": 0.145,
        "Hydrogen": 0.103,
        "Oxygen": 0.735,
        "Nitrogen": 0.030,
        "Silicon": 0.000,
        "Other": 0.012,
        "density": 1.060,
    },

    # Approximate cortical bone composition
    "Bone": {
        "Carbon": 0.270,
        "Hydrogen": 0.040,
        "Oxygen": 0.410,
        "Nitrogen": 0.030,
        "Silicon": 0.000,
        "Other": 0.250,
        "density": 1.850,
    },

    # Approximate adipose tissue
    "Adipose Tissue": {
        "Carbon": 0.598,
        "Hydrogen": 0.114,
        "Oxygen": 0.278,
        "Nitrogen": 0.007,
        "Silicon": 0.000,
        "Other": 0.003,
        "density": 0.950,
    },
}