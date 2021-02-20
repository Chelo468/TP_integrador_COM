using Entidades;
using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.Configuration;
using System.Data;
using System.Data.SqlClient;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DataAccess
{
    public class MensajeDataAccess
    {
        static string TOPIC_GET_CONFIGURATION = "/com/cuna/myConfiguration";

        private static Mensaje MapearMensaje(DataRow lector)
        {
            Mensaje mensaje = new Mensaje();

            mensaje.id_mensaje = int.Parse(lector["id_mensaje"].ToString());
            mensaje.topic = lector["topic"].ToString();
            mensaje.mensaje = lector["mensaje"].ToString();
            mensaje.fecha = DateTime.Parse(lector["fecha"].ToString());

            return mensaje;
        }

        private static string obtenerCadenaConexion()
        {
            return ConfigurationManager.AppSettings["sqlConnection"];
        }
        public static void MensajeInsert(string topic, string mensaje, DateTime fecha)
        {
            List<SqlParameter> parametros = new List<SqlParameter>();

            parametros.Add(new SqlParameter("@topic", topic));
            parametros.Add(new SqlParameter("@mensaje", mensaje));
            parametros.Add(new SqlParameter("@fecha", fecha));

            executeNonQueryProc("MensajesInsert", parametros);
        }

        public static List<Mensaje> GetByTopic(string topic, DateTime fecha_desde, DateTime fecha_hasta)
        {
            List<Mensaje> mensajes = new List<Mensaje>();

            List<SqlParameter> parametros = new List<SqlParameter>();

            parametros.Add(new SqlParameter("@topic", topic));
            parametros.Add(new SqlParameter("@fecha_desde", fecha_desde));
            parametros.Add(new SqlParameter("@fecha_hasta", fecha_hasta));

            DataTable tableResult = executeQueryProc("Mensajes_getByTopic", parametros);

            if(tableResult.Rows.Count > 0)
            {
                for (int i = 0; i < tableResult.Rows.Count; i++)
                {
                    mensajes.Add(MapearMensaje(tableResult.Rows[i]));
                }
            }

            return mensajes;
        }

        

        public static ConfiguracionESP32 getConfiguration(DateTime momentoConsulta)
        {
            Mensaje mensaje = new Mensaje();

            List<SqlParameter> parametros = new List<SqlParameter>();

            parametros.Add(new SqlParameter("@moment", momentoConsulta));
            parametros.Add(new SqlParameter("@topic", TOPIC_GET_CONFIGURATION));

            DataTable tableResult = executeQueryProc("Mensajes_getByTopicAfterMoment", parametros);

            if(tableResult.Rows.Count > 0)
            {
                mensaje = MapearMensaje(tableResult.Rows[0]);
            }

            ConfiguracionESP32 configuracion = new ConfiguracionESP32();
            
            if(!string.IsNullOrEmpty(mensaje.mensaje))
            {
                configuracion = JsonConvert.DeserializeObject<ConfiguracionESP32>(mensaje.mensaje);
            }
            

            return configuracion;

        }

        private static DataTable executeQueryProc(string storedProcedure, List<SqlParameter> parametros = null)
        {
            DataTable tabla = new DataTable("table");

            using (SqlConnection conexion = new SqlConnection(obtenerCadenaConexion()))
            {
                using (SqlCommand comando = new SqlCommand())
                {
                    comando.Connection = conexion;

                    comando.CommandType = System.Data.CommandType.StoredProcedure;
                    comando.CommandText = storedProcedure;

                    if(parametros != null)
                    {
                        comando.Parameters.AddRange(parametros.ToArray());
                    }
                    

                    conexion.Open();

                    tabla.Load(comando.ExecuteReader());

                    conexion.Close();
                }
            }

            return tabla;
        }

      

        private static void executeNonQueryProc(string storedProcedure, List<SqlParameter> parametros = null)
        {
            using (SqlConnection conexion = new SqlConnection(obtenerCadenaConexion()))
            {
                using (SqlCommand comando = new SqlCommand())
                {
                    comando.Connection = conexion;

                    comando.CommandType = System.Data.CommandType.StoredProcedure;
                    comando.CommandText = storedProcedure;

                    if (parametros != null)
                    {
                        comando.Parameters.AddRange(parametros.ToArray());
                    }

                    conexion.Open();

                    comando.ExecuteNonQuery();

                    conexion.Close();
                }
            }
        }
    }
}
