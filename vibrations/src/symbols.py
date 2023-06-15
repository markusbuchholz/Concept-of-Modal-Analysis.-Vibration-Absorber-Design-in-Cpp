from sympy import symbols, Matrix, Eq, det, solve

# Define the symbols
m1, m2 , m = symbols('m1 m2 m')
k1, k2, k = symbols('k1 k2 k')
f1, f2 = symbols('f1 f2')
x1, x2 = symbols('x1 x2')
ddx1, ddx2 = symbols('ddx1 ddx2')
M = Matrix([[m1 + m2, m2], [m2, m2]])  # Define matrix M
K = Matrix([[k1, 0], [0, k2]])  # Define matrix K
x = Matrix([[x1], [x2]])  # Define matrix x
ddx = Matrix([[ddx1], [ddx2]])  # Define matrix x
F = Matrix([[f1], [0]])  # Define matrix F

# Define the equation Mx + Kx = F
eq = Eq(M * ddx + K * x, F)

print (eq)

# Solve the equation
solution = solve(eq, (ddx1, ddx2))

# Print the solution
print("Solution:")
print(f"x1 = {solution[ddx1]}")
print(f"x2 = {solution[ddx2]}")



# Define the symbol
lambda_ = symbols('lambda')

# Define the matrices
M = Matrix([[m, 0], [0, m]])  # Define matrix M
K = Matrix([[2*k, -k], [-k, k]])  # Define matrix K

# Define the characteristic equation
char_eq = Eq(det(K - lambda_ * M), 0)

# Solve the characteristic equation
solutions = solve(char_eq, lambda_)

# Print the solutions
print("Eigenvalues:")

for solution in solutions:
    print(solution)

#k*(3 - sqrt(5))/(2*m)
#k*(sqrt(5) + 3)/(2*m)