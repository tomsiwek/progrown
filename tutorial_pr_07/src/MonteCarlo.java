/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 *
 * @author labszk
 */
public class MonteCarlo
{
    public static void main (String args[])
    {
        long n = 1000000000;
        long s = 0;
        for(int i = 0; i < n; i++)
        {
            double x = 2*Math.random() - 1;
            double y = 2*Math.random() - 1;
            if(x*x + y*y <= 1) s = s + 1;
        }
        System.out.println("pi ~= " + 4.0 * (double) s/ (double) n);
    }
}
