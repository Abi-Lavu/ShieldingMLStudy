import numpy as np 
import pandas as pd

from sklearn.cluster import KMeans
from sklearn.preprocessing import StandardScaler

from materials import materials

rng = np.random.default_rng(7312026)

Elements = ["Hydrogen", "Carbon", "Nitrogen", "Oxygen",  "Silicon"]

MATERIALS_DF = pd.DataFrame.from_dict(materials, orient="index")
