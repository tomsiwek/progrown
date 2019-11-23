/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 *
 * @author labszk
 */
public class SavitzkyGolay
{
    public static void main(String[] args)
    {

        int n = 1024;
        double x[] = new double[n];
        double xprim[] = new double[n];

        // Inicjalizacja danych
        for (int i = 0; i < n; i++)
        {
            x[i] = Math.random();
        }

        // wygladzanie
        // obliczamy mianownik - czynnik normujący
        double a = 1.0/(-3 + 12 + 17 + 12 - 3);

        // ob;liczenia dla 2 pierwszych elementów z lewej strony
        xprim[0] = (17*x[0] + 12*x[1] - 3*x[2]) / (17 + 12 - 3);
        xprim[1] = (12*x[0] + 17*x[1] + 12*x[2] - 3*x[3]) / (12 + 17 + 12 - 3);
        
        // obliczenia dla wszystkich elementów
        for (int i = 2; i < (n - 2); i++)
        {
            xprim[i] = -3*x[i - 2] + 12*x[i - 1] + 17*x[i] + 12*x[i + 1] - 3*x[i + 2];
            xprim[i] = xprim[i] * a;
        }
        
        // obliczenia na 2 ostatnich elementów (z prawej strony)
        xprim[n - 2] = (12*x[n - 1] + 17*x[n - 2] + 12*x[n - 3] - 3*x[n - 4]) / (12 + 17 + 12 - 3);
        xprim[n - 1] = (17*x[n - 1] + 12*x[n - 2] - 3*x[n - 3]) / (17 + 12 - 3);

        // wypisanie krzywej oryginalnej i przefiltrowaniej
        for (int i = 0; i < n; i++)
        {
            System.out.println(i + " " + x[i] + " " + xprim[i]);
        }
    } 
}
