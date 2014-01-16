import java.util.Scanner; // To read data
import java.io.*;         // To throw exceptions

public class Vector {
 
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