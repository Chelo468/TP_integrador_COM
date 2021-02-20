using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Entidades
{
    public class Mensaje
    {
        public int id_mensaje { get; set; }
        public string topic { get; set; }

        private string _mensaje;
        public string mensaje 
        { 
            get 
            {
                if(topic == "/swa/temperatura")
                {
                    if (_mensaje.Contains("ºC") || _mensaje.Contains("°C"))
                    {
                        return _mensaje.Replace("Temperatura: ", "").Replace(" ºC", "").Replace(" °C", "");
                    }
                }
                

                return _mensaje;
                
            } 
            
            set
            {
                _mensaje = value;
            }
        }
        public DateTime fecha { get; set; }
    }
}
