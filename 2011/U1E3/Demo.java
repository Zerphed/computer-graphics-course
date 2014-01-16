public class Demo {
  
  public static void main(String[] args) {
  
    if (args.length < 1) {
      System.out.print("*** No test chosen, exiting..");
      System.exit(0);
    }
  
    switch (Integer.parseInt(args[0]))
    {
      case 0:
      {
        System.out.println("*** Testing Vector class..");
        
        System.out.println("*** Creating two new vectors..");
        Vector vec1 = new Vector(4);
        Vector vec2 = new Vector(3);
        
        System.out.println("*** Reading data into vectors..");
        vec1.read();
        vec2.read();
        
        System.out.println("*** Printing the vectors ..");
        vec1.print();
        vec2.print();
        
        System.out.println("*** Testing addition: vec1.add(vec2)..");
        vec1.add(vec2);
        vec1.print();
        
        System.out.println("*** Testing static addition..");
        Vector ret = Vector.add(vec1, vec2);
        ret.print();
      
        System.out.println("*** Testing substitution: vec1.sub(vec2)..");
        vec1.sub(vec2);
        vec1.print();
        
        System.out.println("*** Testing static substitution..");
        Vector ret2 = Vector.sub(vec1, vec2);
        ret2.print();
        
        System.out.println("*** Testing multiplication: vec1.mult(2)..");
        vec1.mult(2);
        vec1.print();
        
        System.out.println("*** Testing static multiplication..");
        Vector ret3 = Vector.mult(vec1, 3);
        ret3.print();
        
        System.out.println("*** Testing dot product: v1.dot(v2)..");
        System.out.println("Returned: " + vec1.dot(vec2));
        
        System.out.print("*** Tests complete..");
        break;
      }
      
      case 1:
      {
        System.out.println("*** Testing the Matrix class..");
        
        System.out.println("*** Creating two new matrices..");
        Matrix m1 = new Matrix(4,3);
        Matrix m2 = new Matrix(3,2);
        
        System.out.println("*** Reading 4x3 matrix data..");
        m1.read();
        System.out.println("*** Reading 3x2 matrix data..");
        m2.read();
        
        System.out.println("*** Printing the 4x3 matrix..");
        m1.print();
        System.out.println("*** Printing the 3x2 matrix..");
        m2.print();
        
        System.out.println("*** Multiplying the matrices (static) 4x3 * 3x2..");
        Matrix ret = Matrix.mult(m1, m2);
        
        System.out.println("*** Printing the result..");
        ret.print();
        
        System.out.println("*** Multiplying the matrices (non static) 4x3 * 3x2..");
        m1.mult(m2);
        System.out.println("*** Printing the result..");
        m1.print();
        
        System.out.println("*** Testing matrix - vector multiplication..");
        Vector vec = new Vector(2);
        System.out.println("*** Reading vector data..");
        vec.read();
        m1.mult(vec);
        System.out.println("*** Printing the result..");
        m1.print();
        
        System.out.print("*** Tests complete..");
        break;
      }
      case 2:
      {
        System.out.println("*** Testing the Matrix class..");
        
        System.out.println("*** Creating a new matrix..");
        Matrix m1 = new Matrix(3,3);
        
        System.out.println("*** Reading 3x3 matrix data..");
        m1.read();
        
        System.out.println("*** Testing calculation of determinant..");
        float det = m1.det();
        System.out.println("Determinant: " + det);
        
        System.out.print("*** Tests complete..");
        break;
      }
      
      default:
      {
        System.out.print("*** No such test..");
        break;
      }
    }
  }
}