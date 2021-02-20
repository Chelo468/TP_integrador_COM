//using System;
//using System.Collections.Generic;
//using System.Data.SqlClient;
//using System.Linq;
//using System.Runtime.InteropServices;
//using System.Text;
//using System.Threading.Tasks;
//using uPLibrary.Networking.M2Mqtt;
//using uPLibrary.Networking.M2Mqtt.Messages;

//namespace ServicioMQTT
//{
    
//    public class ConectorMQTT
//    {
//        private static ConectorMQTT _instancia;
//        private MqttClient clienteMqtt;
//        private string clientId;
//        private string _sqlConnection;

//        public static ConectorMQTT Instancia()
//        {
//            if(_instancia == null)
//            {
//                _instancia = new ConectorMQTT();


//            }
//            return _instancia;
//        }

//        private ConectorMQTT()
//        {
            
//        }

//        public void conectar(string broker, string sqlConnection)
//        {
//            _sqlConnection = sqlConnection;

//            clienteMqtt = new MqttClient(broker);

//            clienteMqtt.MqttMsgPublishReceived += client_MqttMsgPublishReceived;

//            clientId = Guid.NewGuid().ToString();

//            clienteMqtt.Connect(clientId);
//        }

//        public bool suscribir(string topic)
//        {
//            try
//            {
//                clienteMqtt.Subscribe(new string[] { topic }, new byte[] { 2 });
//            }
//            catch (Exception)
//            {
//                return false;
//            }
            
//            return true;
//        }

//        void client_MqttMsgPublishReceived(object sender, MqttMsgPublishEventArgs e)
//        {
//            DateTime momento = DateTime.Now;

//            string mensajeRecibido = Encoding.UTF8.GetString(e.Message);

//            guardarEnDB(e.Topic, mensajeRecibido, momento);
//        }

//        private void guardarEnDB(string topic, string mensaje, DateTime fecha)
//        {
//            using (SqlConnection conexion = new SqlConnection(_sqlConnection))
//            {
//                using (SqlCommand comando = new SqlCommand())
//                {
//                    comando.Connection = conexion;

//                    comando.CommandType = System.Data.CommandType.StoredProcedure;
//                    comando.CommandText = "MensajesInsert";

//                    comando.Parameters.Add(new SqlParameter("@topic", topic));
//                    comando.Parameters.Add(new SqlParameter("@mensaje", mensaje));
//                    comando.Parameters.Add(new SqlParameter("@fecha", fecha));

//                    conexion.Open();

//                    comando.ExecuteNonQuery();

//                    conexion.Close();
//                }
//            }
//        }
//    }
//}
