using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;
using pGina.Shared.Types;
using System.IO;

using log4net;

namespace pGina.Plugin.HelloPlugin
{
    public class PluginImpl : pGina.Shared.Interfaces.IPluginAuthentication
    {
        
        public string Name
        {
            get { return "Hello"; }
        }

        public string Description
        {
            get { return "Authenticates all users with 'hello' in the username and 'pGina' in the password"; }
        }

        private static readonly Guid m_uuid = new Guid("DC3BF117-8D7C-4CB7-976B-5CF747693EB2");

        public Guid Uuid
        {
            get { return m_uuid; }
        }

        public string Version
        {
            get
            {
                return System.Reflection.Assembly.GetExecutingAssembly().GetName().Version.ToString();
            }
        }

        public void Starting() { }

        public void Stopping() { }

        public BooleanResult AuthenticateUser(SessionProperties properties)
        {
            UserInformation userInfo = properties.GetTrackedSingle<UserInformation>();

            //while (is_lock == true)
            //ok();
           // System.Diagnostics.Process.Start(@"C:/Users/64467/Documents/Visual Studio 2013/Projects/Seetaface/x64/Release/FaceIdentificationTest.exe");
            StreamReader sr = new StreamReader("D:\\myfile.txt", Encoding.Default);
            String line;
            line = sr.ReadLine();
            
            float f1 = Convert.ToSingle(line);
            sr.Close();
            if (f1 > 0.6)
            {
                userInfo.Username = "username";
                userInfo.Password = "password";
                return new BooleanResult() { Success = true };
            }
            else{
                return new BooleanResult() { Success = false, Message = line };
            }
            
        }

        private static dynamic m_settings;
        internal static dynamic Settings { get { return m_settings; } }

        static PluginImpl()
        {
            m_settings = new pGina.Shared.Settings.pGinaDynamicSettings(m_uuid);

            m_settings.SetDefault("Foo", "Bar");
            m_settings.SetDefault("DoSomething", true);
            m_settings.SetDefault("ListOfStuff", new string[] { "a", "b", "c" });
            m_settings.SetDefault("Size", 1);
        }
    }


}
