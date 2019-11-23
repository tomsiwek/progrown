import java.io.IOException;
import java.util.Scanner;
import org.pcj.*;

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 *
 * @author labszk
 */

@RegisterStorage(PCJBroadcast.Shared.class)

public class PCJBroadcast implements StartPoint
{
    double a;
    @Storage(PCJBroadcast.class)
    enum Shared {a};
    Scanner sc = new Scanner(System.in);
    
    public static void main(String args[]) throws IOException
    {
        PCJ.start(PCJBroadcast.class, new NodesDescription("nodes.txt"));
    }

    @Override
    public void main() throws Throwable
    {
        if(PCJ.myId() == 0)
        {
            a = sc.nextDouble();
        }
        else
        {
            a = 0;
        }
        System.out.println("Thread no. " + PCJ.myId() + " a = " + a + ".");
        if(PCJ.myId() == 0)
        {
            PCJ.broadcast(a, Shared.a);
        }
        // PCJ.barrier();
        PCJ.waitFor(Shared.a);
        System.out.println("Thread no. " + PCJ.myId() + " (after broadcast) a = " + a + ".");
    }
    
}
