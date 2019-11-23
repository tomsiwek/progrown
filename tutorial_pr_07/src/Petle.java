/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 *
 * @author labszk
 */
public class Petle
{
    public static void main(String args[])
    {
        int[] x = new int[10];
        
        for(int i = 0; i < 10; i = i + 1)
        {
            x[i] = duplikuj(i);
        }
        
        for(int i = 0; i < 10; i = i + 1)
        {
            System.out.println("x" + i + " = " + x[i]);
        }
    }
    
    static int duplikuj(int x)
    {
        int d = 2*x;
        return d;
    }
}
