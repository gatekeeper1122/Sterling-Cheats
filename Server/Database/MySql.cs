using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using MySql.Data.MySqlClient;

namespace Server {
    class MySQL {
        public static MySqlConnection Setup() {
            return new MySqlConnection(String.Format("Server={0};Port=3306;Database={1};Uid={2};Password={3};", "localhost", "Sterling", "root", ""));
        }

        public static bool Connect(MySqlConnection connection) {
            try {
                connection.Open();
                return true;
            }
            catch (MySqlException exception) {
                Console.WriteLine(exception.Message);
                return false;
            }
        }

        public static void Disconnect(MySqlConnection connection) {
            try {
                connection.Close();
            }
            catch (MySqlException exception) {
                Console.WriteLine(exception.Message);
            }
        }
    }
}
