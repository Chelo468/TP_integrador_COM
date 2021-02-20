using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Web.UI;

namespace Utils
{
    public interface IUpdatable
    {
        void actualizar(ref Page page);

        void btnActualizar_Click(object sender, EventArgs e);
    }
}
