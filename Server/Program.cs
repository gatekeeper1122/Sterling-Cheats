using System;

namespace Server {
    class Program {
        static void Main(string[] args) {
            Console.ForegroundColor = ConsoleColor.White;
            Console.WindowWidth = 150;
            Console.WindowHeight = 31;
            Console.Title = "Sterling Cheats Handler";

            try {
                Console.WriteLine($"Size: {sizeof(bool)}");
                // handles incoming packers
                new ClientHandler().Start();
            } catch(Exception exception) {
                Console.WriteLine("Exception: " + exception.Message);
            }
        }
    }
}
