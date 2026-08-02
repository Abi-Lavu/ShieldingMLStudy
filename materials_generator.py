import numpy as np
import pandas as pd

from sklearn.cluster import KMeans
from sklearn.metrics import silhouette_score
from sklearn.preprocessing import StandardScaler

from materials import materials

SEED = 7312026
rng = np.random.default_rng(SEED)

ELEMENTS = ["Carbon", "Hydrogen", "Oxygen", "Nitrogen", "Silicon"]
N_CLUSTERS = 6
N_SYNTHETIC = 1000
COMPOSITION_NOISE_STD = 0.02
DENSITY_NOISE_STD = 0.05

MATERIALS_DF = pd.DataFrame.from_dict(materials, orient="index")

FEATURES = ELEMENTS + ["density"]
X = StandardScaler().fit_transform(MATERIALS_DF[FEATURES])

kmeans = KMeans(n_clusters=N_CLUSTERS, random_state=SEED, n_init=10)
MATERIALS_DF["cluster"] = kmeans.fit_predict(X)
SILHOUETTE_SCORE = silhouette_score(X, MATERIALS_DF["cluster"])

COMPOSITION = MATERIALS_DF[ELEMENTS].div(MATERIALS_DF[ELEMENTS].sum(axis=1), axis=0)


def blend_cluster(cluster_id: int) -> dict:
    """Dirichlet-blend every material in a cluster into one synthetic candidate."""
    members = MATERIALS_DF.index[MATERIALS_DF["cluster"] == cluster_id]
    weights = rng.dirichlet(np.ones(len(members)))

    blended = COMPOSITION.loc[members].to_numpy().T @ weights
    blended = blended + rng.normal(0.0, COMPOSITION_NOISE_STD, size=blended.shape)
    blended = np.clip(blended, 0.0, None)
    blended = blended / blended.sum()

    density = float(MATERIALS_DF.loc[members, "density"].to_numpy() @ weights)
    density *= 1.0 + rng.normal(0.0, DENSITY_NOISE_STD)

    row = dict(zip(ELEMENTS, blended))
    row["density"] = max(density, 0.01)
    row["cluster"] = cluster_id
    row["source_materials"] = ", ".join(members)
    return row

cluster_draws = rng.integers(0, N_CLUSTERS, size=N_SYNTHETIC)
SYNTHETIC_DF = pd.DataFrame(blend_cluster(cluster_id) for cluster_id in cluster_draws)

if __name__ == "__main__":
    print(f"K={N_CLUSTERS} k-means silhouette score: {SILHOUETTE_SCORE:.3f}")
    for cluster_id in range(N_CLUSTERS):
        members = MATERIALS_DF.index[MATERIALS_DF["cluster"] == cluster_id].tolist()
        print(f"  cluster {cluster_id} ({len(members)}): {', '.join(members)}")

    print(f"\nGenerated {len(SYNTHETIC_DF)} synthetic materials")
    print(SYNTHETIC_DF["cluster"].value_counts().sort_index())

    SYNTHETIC_DF.to_csv("synthetic_materials.csv", index=False)
    print("\nWrote synthetic_materials.csv")
