import java.io.FileWriter;
import java.io.IOException;
import java.io.Writer;
import java.util.logging.Level;
import java.util.logging.Logger;
/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 *
 * @author labszk
 */
public class HeatTransfer
{
     public static void main(String[] args)
     {

        // Material
        double rho = 7800;              // density
        double c = 500;                 // heat capacity
        double lambda = 46;             // thermal conductivity

        double a = lambda / (rho * c);  //

        // Parametry płytki AxB
        double A = 0.32;             // height
        double B = 1.6;              // width
        double t = 1000;             // time

        // Bordering
        double T0 = 20;             // initail temperature
        double hx = 0.01;           // x step
        double hy = 0.01;           // y step
        double tau = 1 / (a * 12 * (1 / (hx * hx) + 1 / (hy * hy)));     // time step (calculated as maximal step of explicit method)
        
        int Nx = Math.round((float) (B / (hx)));             // size of x grid
        int Ny = Math.round((float) (A / (hy)));             // size of y grid
        int Nt = Math.round((float) (t / tau));              // number of time steps

        double T[][] = new double[Nx][Ny];                   // Temparature at time k
        double Tnew[][] = new double[Nx][Ny];                // Temperature at time k+1
        // Set initial temparature
        for (int i = 0; i < Nx; i++)
        {
            for (int j = 0; j < Ny; j++)
            {
                T[i][j] = T0;
            }
        }

        // Border conditions (constatnt temperature on borders)
        double Tl = 1200;
        double Tr = 1200;
        double Tu = 1200;
        double Td = 1200;

        for(double k = 0.0; k <= t; k = k + tau)
        {                                              
            for (int i = 0; i < Nx; i++)
            {
                for (int j = 0; j < Ny; j++)
                {
                    if (i == 0 && j == 0)
                    {
                        Tnew[i][j] = T[i][j] + 2 * a * tau * (((T[i + 1][j] + T[i][j + 1] - 2 * T[i][j]) / (hx * hx)) + (Tl + Td) / (lambda * hx));
                    }
                    else if (i == 0 && j == Ny - 1)
                    {
                        Tnew[i][j] = T[i][j] + 2 * a * tau * (((T[i + 1][j] + T[i][j - 1] - 2 * T[i][j]) / (hx * hx)) + (Tl + Tu) / (lambda * hx));
                    }
                    else if (i == Nx - 1 && j == 0)
                    {
                        Tnew[i][j] = T[i][j] + 2 * a * tau * (((T[i - 1][j] + T[i][j + 1] - 2 * T[i][j]) / (hx * hx)) + (Tr + Td) / (lambda * hx));
                    }
                    else if (i == Nx - 1 && j == Ny - 1)
                    {
                        Tnew[i][j] = T[i][j] + 2 * a * tau * (((T[i - 1][j] + T[i][j - 1] - 2 * T[i][j]) / (hx * hx)) + (Tr + Tu) / (lambda * hx));
                    }
                    else if (i == 0)
                    {             // Left  border                 
                        Tnew[i][j] = T[i][j] + a * tau * (T[i][j + 1] - 2 * T[i][j] + T[i][j - 1]) + (Tl + (lambda * (T[i + 1][j] - T[i][j]) / hx)) * (2 * tau / (c * rho * hx));
                    }
                    else if (i == Nx - 1)
                    {              // Right  border
                        Tnew[i][j] = T[i][j] + a * tau * (T[i][j + 1] - 2 * T[i][j] + T[i][j - 1]) + (Tr + (lambda * (T[i - 1][j] - T[i][j]) / hx)) * (2 * tau / (c * rho * hx));
                    }
                    else if (j == 0)
                    {                 // Lower border
                        Tnew[i][j] = T[i][j] + a * tau * (T[i + 1][j] - 2 * T[i][j] + T[i - 1][j]) + (Td + (lambda * (T[i][j + 1] - T[i][j]) / hx)) * (2 * tau / (c * rho * hx));
                    }
                    else if (j == Ny - 1)
                    {              // Top border
                        Tnew[i][j] = T[i][j] + a * tau * (T[i + 1][j] - 2 * T[i][j] + T[i - 1][j]) + (Tu + (lambda * (T[i][j - 1] - T[i][j]) / hx)) * (2 * tau / (c * rho * hx));
                    }
                    else
                    {                                  // internal points
                        Tnew[i][j] = T[i][j] + a * tau * ((T[i + 1][j] - 2 * T[i][j] + T[i - 1][j]) / (hx * hx) + (T[i][j + 1] - 2 * T[i][j] + T[i][j - 1]) / (hy * hy));
                    }
                }
            }
            System.out.println(k);
            T = Tnew;             // Update temperature table (k -> k+1)
        }
        
        // output to file
        Writer out;
        try
        {
            out = new FileWriter("out.txt");
            for (int i = 0; i < Nx; i++)
            {
                for (int j = 0; j < Ny; j++)
                {
                    out.write(T[i][j] + " ");
                }
                out.write("\n");
            }
            out.close();
        }
        catch (IOException ex)
        {
            Logger.getLogger(HeatTransfer.class.getName()).log(Level.SEVERE, null, ex);
        }
    }
}
