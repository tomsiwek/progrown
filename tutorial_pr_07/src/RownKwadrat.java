/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 *
 * @author labszk
 */

import java.util.Scanner;

public class RownKwadrat
{
    public static void main(String args[])
    {
        double a;
        double b;
        double c;
        
        Scanner sc = new Scanner(System.in);
        
        a = sc.nextDouble();
        b = sc.nextDouble();
        c = sc.nextDouble();
        
        double delta = b*b - 4.0*a*c;
        
        if(delta < 0.0)
        {
            System.out.println("Brak pierwiastków w R.");
        }
        else if (delta == 0.0)
        {
            System.out.println("Jeden pierwiastek w R.");
            double x0 = -b/(2.0*a);
            System.out.println(x0);
        }
        else
        {
            System.out.println("Dwa pierwiastki w R.");
            double x1 = (-b - Math.sqrt(delta))/(2.0*a);
            double x2 = (-b + Math.sqrt(delta))/(2.0*a);
            System.out.println(x1 + " " + x2);
        }
    }
    
}
