import java.io.IOException;
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

public class PCJHelloWorld implements StartPoint
{
    
    public static void main(String args[]) throws IOException
    {
        PCJ.start(PCJHelloWorld.class, new NodesDescription("nodes.txt"));
    }

    @Override
    public void main() throws Throwable {
        System.out.println("Hello from " + PCJ.myId() + " out of " + PCJ.threadCount() + ".");
    }
    
}
