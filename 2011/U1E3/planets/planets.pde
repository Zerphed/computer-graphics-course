import java.util.Scanner; // To read data
import java.io.*;         // To throw exceptions
import java.lang.Math.*;  // To calculate the powers easier

int SCREEN_W = 800;
int SCREEN_H = 600;

PFont fontA;

public static class Matrix {
  
  private int rows;
  private int cols;
  private float data[][];
  
  public Matrix(int rows, int cols) {
    if (rows < 1|| cols < 1)
      throw new RuntimeException("Matrix dimensions out of bounds (rows or cols < 1)");
  
    this.rows = rows;
    this.cols = cols;
    this.data = new float[rows][cols];
    
    for (int i = 0; i < rows; i++)
      for (int j = 0; j < cols; j++)
        this.data[i][j] = 0;
  }
  
  
  public Matrix(int rows, int cols, float[][] data)
  {
    if (rows < 1 || cols < 1)
      throw new RuntimeException("Matrix dimensions out of bounds (rows or cols < 1)");
      
    this.rows = rows;
    this.cols = cols;
    this.data = new float[rows][cols];
    
    for (int i = 0; i < rows; i++)
      for (int j = 0; j < rows; j++)
        this.data[i][j] = data[i][j];
  }
  
  
  public int rows() {
    return this.rows;
  }
  
  
  public int cols() {
    return this.cols; 
  }
  
  
  public float[][] data() {
    return this.data;
  }
  
  
  public void mult(Matrix other) {
    if (this.cols != other.rows)
      throw new RuntimeException("Multiplication error: illegal matrix dimensions for multiply");
    
    float[][] ret = new float[other.rows][this.cols];
    
    for (int i = 0; i < this.rows; i++)
      for (int j = 0; j < other.cols; j++)
        for (int k = 0; k < this.cols; k++)
          ret[i][j] += this.data[i][k] * other.data[k][j];
    
    this.data = ret;
    this.cols = other.cols;
  }
  
  
  public static Matrix mult(Matrix m1, Matrix m2) {
    if (m1.cols != m2.rows)
      throw new RuntimeException("Multiplication error: illegal matrix dimensions for multiply");

    Matrix ret = new Matrix(m1.rows, m2.cols);
      
    for (int i = 0; i < m1.rows; i++)
      for (int j = 0; j < m2.cols; j++)
        for (int k = 0; k < m1.cols; k++)
          ret.data[i][j] += m1.data[i][k] * m2.data[k][j];
          
    return ret;
  }
 
  
  public void mult(Vector vec) {
    if (this.cols != vec.dimension())
      throw new RuntimeException("Multiplication error: illegal matrix - vector dimensions for multiply");

    float[] vecData = vec.data();
    float[][] newData = new float[this.rows][1];
    
    for (int i = 0; i < this.rows; i++)
      for (int j = 0; j < this.cols; j++)
        newData[i][0] += this.data[i][j] * vecData[j];
        
    this.data = newData;
    this.cols = 1;
  }
  
  
  public static Matrix mult(Matrix m, Vector vec) {
    if (m.cols != vec.dimension())
      throw new RuntimeException("Multiplication error: illegal matrix - vector dimensions for multiply");

    Matrix ret = new Matrix(m.rows, 1);
    float[] vecData = vec.data();
    
    for (int i = 0; i < m.rows; i++)
      for (int j = 0; j < m.cols; j++)
        ret.data[i][0] += m.data[i][j] * vecData[j];
          
    return ret;
  }
  
  
  public Matrix identity() {
    
    Matrix identity = new Matrix(this.rows, this.cols);
    
    for (int i = 0, j = 0; i < this.rows && j < this.cols; i++, j++)
      identity.data[i][j] = 1;

    return identity;
  }
  
  // THIS STILL NEEDS FIXING
  public Matrix invert() {
    
    if (this.det() == 0)
      throw new RuntimeException("Inversion error: the matrix is singular i.e. determinant is zero");

    // Get the LU decomposition of the matrix
    Matrix[] lu = this.lu();
    Matrix l = lu[0];
    Matrix u = lu[1];
    
    // First find the inverse of the L matrix by gaussian
    // elimination
    Matrix l_inv = l.identity();
    
    // Insert lower triangular gaussian elim. here
    float pivot;
    
    System.out.println("Matrix L:");
    l.print();
    System.out.println("Matrix U:");
    u.print();
    System.out.println("Inverting L:");
    
    for (int i = 0; i < this.rows; i++)
    {
      pivot = l.data[i][i];
      // Make the current pivot value one
      for (int j = 0; j < this.cols; j++)
      {
        l.data[i][j] /= pivot;      // Make the pivot value one
        l_inv.data[i][j] /= pivot;  // Duplicate all the operations for the id matrix
      }
      
      // Destroy the pivot column from the other rows
      for (int k = i+1; k < this.rows; k++) {
        for (int j = 0; j < this.cols; j++) {
          float to_add = l.data[k][j] * (-1); // Find the value to add to the rows below
          l.data[k][j] += to_add;
          l_inv.data[k][j] += to_add;
        }
      }
    }
        
    l_inv.print();
    System.out.print("\n");
    l.print();
    System.out.print("\n");
    
    // Second find the inverse of the U matrix by gaussian
    // elimination
    Matrix u_inv = u.identity();
    
    // Insert upper triangular gaussian elim. here
    
    // Third the inverse is A^-1 = U^-1 * L^-1
    return Matrix.mult(u_inv, l_inv);
  }
  
  
  public float det() {
    // Check that the matrix in question is square
    if (this.rows != this.cols)
      throw new RuntimeException("Determinant calculation error: the matrix is not square");
    
    // So it's an n x n square matrix
    int n = this.rows;
    float det = 0;
    
    // If the size of the matrix is one, its special
    if (n == 1)
      det = this.data[0][0];
      
    // If the size is two the determinant is calculated by: ad - bc
    else if (n == 2)
      det = this.data[0][0] * this.data[1][1] - this.data[1][0] * this.data[0][1];
      
    // If the size is bigger than two, rely on recursion
    else 
    {
      for (int i = 0; i < n; i++) 
      {
        // We need to create a new n-1 x n-1 submatrix
        Matrix submatrix = new Matrix(n-1, n-1);
        
        // So basically for every submatrix of this matrix
        // call this function recursively
        for (int j = 1; j < n; j++) 
        {   
            // Copy the appropriate values to the submatrix's
            // data
            for (int k = 0, m = 0; k < n; k++) 
            {
               if (k == i) continue;
               submatrix.data[j-1][m] = this.data[j][k];
               m++;
            }
         }
         det += Math.pow(-1.0, i + 2.0) * this.data[0][i] * submatrix.det();
      }
    }
    return det;
  }
  
  // THIS STILL NEEDS FIXING
  public Matrix[] lu()
  {
    if (this.rows != this.cols)
      throw new RuntimeException("Invalid matrix for LU decomposition, the matrix is not square");
         
    int n = this.rows;
    
    // Initializing the L and U matrices
    float[][] l = new float[n][n];
    float[][] u = new float[n][n];
    
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        if (i == j)
        {
          l[i][j] = 1.0;
          u[i][j] = 0.0;
        }
        else
          l[i][j] = u[i][j] = 0.0;
      }
    }
    
    // Forming the L and U matrices by pivoting
    for (int k = 0; k < n; k++) {
      u[k][k] = this.data[k][k];
      for (int i = k + 1; i < n; i++) {
        l[i][k] = this.data[i][k] / u[k][k];
        u[k][i] = this.data[k][i];
      }
      for (int i = k + 1; i < n; i++) {
        for (int j = k + 1; j < n; j++) {
          this.data[i][j] = this.data[i][j] - (l[i][k] * u[k][j]);
        }
      }
    }
    
    // Indices: 0 = L matrix, 1 = U matrix
    Matrix lm = new Matrix(n, n, l);
    Matrix um = new Matrix(n, n, u);
    return new Matrix[]{lm, um};
  }
  
  
  public static Matrix rotate(float deg) {
    float rad = radians(deg);
    return new Matrix(3, 3, new float[][]{{cos(rad), -sin(rad), 0}, {sin(rad), cos(rad), 0}, {0, 0, 1}});
  }
  
  
  public static Matrix translate(float x, float y) {
    return new Matrix(3, 3, new float[][]{{1, 0, x}, {0, 1, y},{0, 0, 1}});
  }
  
  
  public void read() {
    Scanner sc = new Scanner(System.in);
    
    for (int i = 0; i < this.rows; i++)
      for (int j = 0; j < this.cols; j++)
        this.data[i][j] = sc.nextFloat();
  }
  
  
  public void print() {
    for (int i = 0; i < this.rows; i++)
      for (int j = 0; j < this.cols; j++)
        if (this.cols == 1)
          System.out.println("|" + this.data[i][j] + "|");
        else if (j == 0)
          System.out.print("|" + this.data[i][j] + " ");
        else if (j < (this.cols-1))
          System.out.print(this.data[i][j] + " ");
        else
          System.out.println(this.data[i][j] + "|");
  }
  
}

public static class Vector {
 
  private final int dimension;
  private float data[];
  
  
  public Vector(int dimension) {
    if (dimension < 1)
      throw new RuntimeException("Vector dimension out of bounds (dimension < 1)");
  
    this.dimension = dimension;
    this.data = new float[dimension];
  
    for (int i = 0; i < dimension; i++)
      data[i] = 0;
  }
  
  
  public Vector(float[] data)
  {
    if (data.length < 1)
      throw new RuntimeException("Vector dimension out of bounds (dimension < 1)");
      
    this.dimension = data.length;
    this.data = data;
  }
 
 
  public int dimension() {
    return this.dimension; 
  }
 
 
  public float[] data() {
    return this.data;
  }
  
  
  public void add(Vector other) {
    if (this.dimension != other.dimension)
      throw new RuntimeException("Addition error: vector dimensions do not match.");
      
    for (int i = 0; i < this.dimension; i++)
      this.data[i] += other.data[i];
  }
  
  
  public static Vector add(Vector v1, Vector v2) {
    if (v1.dimension != v2.dimension)
      throw new RuntimeException("Addition error: vector dimensions do not match.");
      
    Vector ret = new Vector(v1.dimension);
    for (int i = 0; i < v1.dimension; i++)
      ret.data[i] = v1.data[i] + v2.data[i];
      
    return ret;
  }
  
  
  public void sub(Vector other) {
    if (this.dimension != other.dimension)
      throw new RuntimeException("Substitution error: vector dimensions do not match.");
      
    for (int i = 0; i < this.dimension; i++)
      this.data[i] -= other.data[i];
  }
  
  
  public static Vector sub(Vector v1, Vector v2) {
    if (v1.dimension != v2.dimension)
      throw new RuntimeException("Substitution error: vector dimensions do not match.");
      
    Vector ret = new Vector(v1.dimension);
    for (int i = 0; i < v1.dimension; i++)
      ret.data[i] = v1.data[i] - v2.data[i];
      
    return ret;
  }
  
  
  public void mult(float scalar) {
    for (int i = 0; i < this.dimension; i++)
      this.data[i] *= scalar;
  }
  
  
  public static Vector mult(Vector vec, float scalar) {
    Vector ret = new Vector(vec.dimension);
    
    for (int i = 0; i < vec.dimension; i++)
      ret.data[i] = vec.data[i] * scalar;
      
    return ret;
  }
  
  
  public float dot(Vector other) {
    if (this.dimension != other.dimension)
      throw new RuntimeException("Dot product error: vector dimensions do not match.");

    float ret = 0;
    
    for (int i = 0; i < this.dimension; i++)
      ret += (this.data[i] * other.data[i]);
      
    return ret;
  }
  
  public void read() {
    
    Scanner sc = new Scanner(System.in);
    
    for (int i = 0; i < this.dimension; i++)
      this.data[i] = sc.nextFloat();
  }
  
  public void print() {
    System.out.print("[");
    for (int i = 0; i < this.dimension; i++)
      if (i != this.dimension - 1)
        System.out.print(this.data[i] + ", ");
      else
        System.out.println(this.data[i] + "]");
  }

}

void setup() {
  size(SCREEN_W, SCREEN_H);
  smooth();
  
  // Set the font and its size (in units of pixels)
  fontA = loadFont("data/Ziggurat-HTF-Black-32.vlw");
  textFont(fontA, 15);
}

void draw()
{
  background(100);
  
  Vector o = new Vector(new float[]{0, 0, 1});

  Matrix t_a = Matrix.translate(400, 300);
  Matrix t_k = Matrix.translate(80, 0);
  Matrix t_p = Matrix.translate(250, 10);

  Matrix r_k = Matrix.rotate(millis() / 125.0);
  Matrix r_p = Matrix.rotate(millis() / 250.0);
  
  // Sun middle
  ellipse(t_a.data()[0][2], t_a.data()[1][2], 80, 80);
  
  // Planet middle
  Matrix p_mid = Matrix.mult(Matrix.mult(t_a, r_p), Matrix.mult(t_p, o));
  ellipse(p_mid.data()[0][0], p_mid.data()[1][0], 60, 60);
  
  // Moon middle
  Matrix m_mid = Matrix.mult(Matrix.mult(Matrix.mult(t_a, r_p), Matrix.mult(t_p, r_k)), Matrix.mult(t_k, o));
  ellipse(m_mid.data()[0][0], m_mid.data()[1][0], 40, 40);
  
  fill(0);
  textAlign(RIGHT);
  text("Sun X: " + (int)t_a.data()[0][2] + " Sun Y: " + (int)t_a.data()[1][2] + "\n" + "Planet X: " + (int)p_mid.data()[0][0] + " Planet Y: " + (int)p_mid.data()[1][0] + "\n" + "Moon X: " + (int)m_mid.data()[0][0] + " Moon Y: " + (int)m_mid.data()[1][0], SCREEN_W - 15, 20);
}
