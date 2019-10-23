import java.io.FileWriter;
import java.io.IOException;
import java.io.Writer;
import java.util.logging.Level;
import java.util.logging.Logger;
import org.pcj.StartPoint;
import org.pcj.PCJ;
import org.pcj.NodesDescription;
import org.pcj.RegisterStorage;
import org.pcj.Storage;
/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 *
 * @author labszk
 */
@RegisterStorage(PCJHeatTransfer.Shared.class)

public class PCJHeatTransfer implements StartPoint
{ 
    @Storage(PCJHeatTransfer.class)
    enum Shared {T};
    double T[][];
    
    public static void main (String args[]) throws IOException
    {
        PCJ.start(PCJHeatTransfer.class, new NodesDescription("nodes.txt"));
    }
    
    @Override
    public void main() throws Throwable
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
        
        // Sprawdzić, czy liczba procesorów jest p*p
        int pp = (int)Math.sqrt(PCJ.threadCount());
        if(pp*pp != PCJ.threadCount())
        {
            System.out.println("Nieodpowiednia liczba procesorów!");
            System.exit(1);
        }

        T = new double[Nx][Ny];                 // Temparature at time k
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
//            System.out.println(k);
            T = Tnew;             // Update temperature table (k -> k+1)
        }
        
        // Zapis do pliku
        Writer out;
        
        // Synchroniczne zapisywanie danych
         if (PCJ.myId() == 0) {
            try {
                String file = "out.txt";
                out = new FileWriter(file);

                for (int py = 0; py < pp; py++)
                {
                    for (int i = 0; i < Nx; i++)
                    {
                        for (int px = 0; px < pp; px++)
                        {
                            for (int j = 0; j < Ny; j++)
                            {
                                out.write((double) PCJ.get(px+pp*py, Shared.T, i, j) + " ");
                            }
                        }
                        out.write("\n");
                    }
                }

                out.close();
            } catch (IOException ex) {
                Logger.getLogger(HeatTransfer.class.getName()).log(Level.SEVERE, null, ex);
            }
        }
    }
}
