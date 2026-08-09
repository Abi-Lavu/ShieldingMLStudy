import pandas as pd
import numpy as np

from sklearn.ensemble import RandomForestRegressor
from sklearn.metrics import mean_squared_error, r2_score


train_df = pd.read_csv("training_data.csv")
test_df = pd.read_csv("testing_data.csv")


def parse_composition(composition):
    result = {}

    for item in str(composition).split(";"):
        if not item.strip():
            continue

        element, fraction = item.split(":")
        result[element] = float(fraction)

    return result


def prepare_data(df):
    composition_df = (df["composition"].apply(parse_composition).apply(pd.Series).add_prefix("composition_"))
    df = pd.concat([df.drop(columns=["composition"]), composition_df], axis=1)
    return df.fillna(0)


train_df = prepare_data(train_df)
test_df = prepare_data(test_df)


target = "mean_edep_lead_MeV"

composition_features = [col for col in train_df.columns if col.startswith("composition_")]
feature_columns = ["areal_density_gcm2", "thickness_cm", "shield_density_gcm3", "energy_MeV"] + composition_features


X_train = train_df[feature_columns]
y_train = train_df[target]

X_test = test_df[feature_columns]
y_test = test_df[target]


model = RandomForestRegressor(n_estimators=300, max_depth=None, min_samples_leaf=2,random_state=42, n_jobs=-1)
model.fit(X_train, y_train)

y_pred = model.predict(X_test)


rmse = np.sqrt(mean_squared_error(y_test, y_pred))
r2 = r2_score(y_test, y_pred)

print(f"Training samples: {len(X_train)}")
print(f"Testing samples: {len(X_test)}")
print()
print(f"RMSE: {rmse:.6f} MeV")
print(f"R²:   {r2:.6f}")
