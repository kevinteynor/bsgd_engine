using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.Diagnostics;
using System.IO;

namespace QtAutoMOC
{
    class Program
    {
        // args[0] is the filepath for the project to moc.
        // args[1] is the filepath for moc.exe
        static void Main(string[] args)
        {
            // Get parameters
            FileInfo projectFile = new FileInfo(args[0]);
            FileInfo qtMocFile = new FileInfo(args[1]);
            DirectoryInfo projectDir = projectFile.Directory;

            // find the header files
            IEnumerable<FileInfo> HeaderFiles = projectDir.GetFiles().Where(a => a.Extension == ".h");
            List<string> createdMocFiles = new List<string>();

            foreach (FileInfo hfile in HeaderFiles)
            {
                StreamReader reader = new StreamReader(hfile.FullName);
                string filebody = reader.ReadToEnd();
                reader.Close();

                FileInfo outputfile = new FileInfo(hfile.FullName.Replace(hfile.Extension, "") + "_moc.cpp");

                // find h files that need to be moc'ed
                if (filebody.Contains("Q_OBJECT"))
                {
                    string arguments = "\"" + hfile.FullName + "\"" + 
                        " -o " +
                        "\"" + outputfile.FullName + "\"";

                    Process.Start(qtMocFile.FullName, @arguments);
                    createdMocFiles.Add(outputfile.Name);

                    // try to find the corresponding CPP file for each moc'ed h-file and #include the new file at the end.
                    string cppFile = hfile.FullName.Replace(hfile.Extension, "") + ".cpp";
                    if (File.Exists(cppFile))
                    {
                        if (!File.ReadAllLines(cppFile).Contains("#include \"" + outputfile.Name + "\""))
                        {
                            var appender = File.AppendText(cppFile);
                            appender.WriteLine();
                            appender.Write("#include \"" + outputfile.Name + "\"");
                            appender.Flush();
                            appender.Close();
                        }
                    }

                }
            }
        }
    }
}
