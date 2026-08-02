import numpy as np
import matplotlib.pyplot as plt

from sklearn.linear_model import LinearRegression
from sklearn.tree import DecisionTreeRegressor
from sklearn.svm import SVR
from sklearn.ensemble import RandomForestRegressor
from sklearn.metrics import mean_squared_error, root_mean_squared_error

np.random.seed(8102026)
X = np.linspace(-3, 3, 100).reshape(-1, 1)

y_true = 0.5 * X[:, 0]**3 - 2 * X[:, 0]**2 + X[:, 0] + 3

noise = np.random.normal(0, 2, size=len(X))
y = y_true + noise


X_poly = np.column_stack([X[:, 0], X[:, 0]**2, X[:, 0]**3])


linear_model = LinearRegression()

tree_overfit = DecisionTreeRegressor(max_depth=None, random_state=42)
tree_underfit = DecisionTreeRegressor(max_depth=2, random_state=42)

svm_model = SVR(kernel="rbf", C=100, gamma="scale", epsilon=0.1)

forest_model = RandomForestRegressor(n_estimators=100, max_depth=5, random_state=42)


linear_model.fit(X_poly, y)
tree_overfit.fit(X, y)
tree_underfit.fit(X, y)
svm_model.fit(X, y)
forest_model.fit(X, y)

X_plot = np.linspace(-3, 3, 500).reshape(-1, 1)

X_plot_poly = np.column_stack([X_plot[:, 0], X_plot[:, 0]**2, X_plot[:, 0]**3])


linear_pred = linear_model.predict(X_plot_poly)
tree_overfit_pred = tree_overfit.predict(X_plot)
tree_underfit_pred = tree_underfit.predict(X_plot)
svm_pred = svm_model.predict(X_plot)
forest_pred = forest_model.predict(X_plot)


plt.figure(figsize=(12, 8))

plt.scatter(X, y, label="Noisy Data", alpha=0.6)
plt.plot(X_plot, 0.5 * X_plot[:, 0]**3- 2 * X_plot[:, 0]**2+ X_plot[:, 0]+ 3,label="True Polynomial")
plt.plot(X_plot, linear_pred, label="Polynomial Linear Regression")
plt.xlabel("X")
plt.ylabel("Y")
plt.title("Regression Model Comparison")
plt.legend()
plt.show()

plt.scatter(X, y, label="Noisy Data", alpha=0.6)
plt.plot(X_plot, 0.5 * X_plot[:, 0]**3- 2 * X_plot[:, 0]**2+ X_plot[:, 0]+ 3,label="True Polynomial")
plt.plot( X_plot, tree_overfit_pred, label="Decision Tree (Overfit)")
plt.plot(X_plot, tree_underfit_pred, label="Decision Tree (Underfit)")
plt.legend()
plt.xlabel("X")
plt.ylabel("Y")
plt.title("Regression Model Comparison")
plt.show()

plt.scatter(X, y, label="Noisy Data", alpha=0.6)
plt.plot(X_plot, 0.5 * X_plot[:, 0]**3- 2 * X_plot[:, 0]**2+ X_plot[:, 0]+ 3,label="True Polynomial")
plt.plot(X_plot, svm_pred, label="SVM")
plt.legend()
plt.xlabel("X")
plt.ylabel("Y")
plt.title("Regression Model Comparison")
plt.show()

plt.scatter(X, y, label="Noisy Data", alpha=0.6)
plt.plot(X_plot, 0.5 * X_plot[:, 0]**3- 2 * X_plot[:, 0]**2+ X_plot[:, 0]+ 3,label="True Polynomial")
plt.plot(X_plot, forest_pred, label="Random Forest")
plt.legend()
plt.xlabel("X")
plt.ylabel("Y")
plt.title("Regression Model Comparison")
plt.show()

print("R^2 Scores:")
print(f"Polynomial Linear Regression: {linear_model.score(X_poly, y):.4f}")
print(f"Decision Tree (Overfit): {tree_overfit.score(X, y):.4f}")
print(f"Decision Tree (Underfit): {tree_underfit.score(X, y):.4f}")
print(f"SVM: {svm_model.score(X, y):.4f}")
print(f"Random Forest: {forest_model.score(X, y):.4f}")

print("RMSE")
print(f"Polynomial Linear Regression: {root_mean_squared_error(y, linear_model.predict(X_poly)):.4f}")
print(f"Decision Tree (Overfit): {root_mean_squared_error(y, tree_overfit.predict(X)):.4f}")
print(f"Decision Tree (Underfit): {root_mean_squared_error(y, tree_underfit.predict(X)):.4f}")
print(f"SVM: {root_mean_squared_error(y, svm_model.predict(X)):.4f}")
print(f"Random Forest: {root_mean_squared_error(y, forest_model.predict(X)):.4f}")
