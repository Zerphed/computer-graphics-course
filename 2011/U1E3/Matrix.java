import java.util.Scanner; // To read data
import java.io.*;         // To throw exceptions
import java.lang.Math.*;     // To calculate the powers easier

public class Matrix {
  
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
    
    this.data = mult(this, other).data;
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
  
  public Matrix invert() {
    
    if (this.det == 0)
      throw new RuntimeException("Inversion error: the matrix is singular i.e. determinant is zero");

    // TODO: IMPLEMENT MEEEEEE!
    
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