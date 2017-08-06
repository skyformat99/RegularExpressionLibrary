using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using IF_MDLLib;
using System.Runtime.InteropServices;


namespace TestIF_MDL
{
    public partial class Form1 : Form
    {
        private IF_MDLLib.IFRegExp reg;
        private IF_MDLLib.IFRegVender vdr;

        public Form1()
        {
            InitializeComponent();

            reg = new IF_MDLLib.IFRegExp();
            reg.OnMatch += reg_OnMatch;
            TRegExpOption opt = reg.RegOption;
            btnOptM.Checked = (opt & TRegExpOption.optM) == TRegExpOption.optM;
            btnOptS.Checked = (opt & TRegExpOption.optS) == TRegExpOption.optS;
            btnOptI.Checked = (opt & TRegExpOption.optI) == TRegExpOption.optI;
            btnOptX.Checked = (opt & TRegExpOption.optX) == TRegExpOption.optX;

            vdr = reg.Vender;
            vdr.MemorySize = TRegExpMemorySize.msDoubleByte;
        }

        void reg_OnMatch(object Sender, int Begin, int End, ref bool Next)
        {
            richTextBox1.SelectionLength = 0;
            richTextBox1.SelectionStart = Begin;
            richTextBox1.SelectionLength = End - Begin;
            richTextBox1.SelectionBackColor = Color.LightGreen;
            System.Drawing.Font font = richTextBox1.SelectionFont;
            richTextBox1.SelectionFont = font != null ? new Font(font.FontFamily, font.Size, FontStyle.Regular) : SystemFonts.DefaultFont;
        }

        private void toolStripButton1_Click(object sender, EventArgs e)
        {
            if (openFileDialog1.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                richTextBox1.LoadFile(openFileDialog1.FileName, RichTextBoxStreamType.PlainText);
            }
        }

        private void toolStripButton2_Click(object sender, EventArgs e)
        {
            int caret = richTextBox1.SelectionStart;
            richTextBox1.SelectionStart = 0;
            richTextBox1.SelectionLength = richTextBox1.TextLength;
            richTextBox1.SelectionBackColor = Color.White;
            System.Drawing.Font font = richTextBox1.SelectionFont;
            richTextBox1.SelectionFont = font != null ? new Font(font.FontFamily, font.Size, FontStyle.Regular) : SystemFonts.DefaultFont;
            richTextBox1.SelectionLength = 0;
            richTextBox1.SelectionStart = caret;

            try
            {
                reg.Pattern = textBox1.Text;
            }
            catch
            {
                textBox1.BackColor = Color.Red;
                showReg(treeView2.Nodes, reg.ParserString);
                treeView2.ExpandAll();
                showReg(treeView3.Nodes, reg.PatternString);
                treeView3.ExpandAll();
                return;
            }

            textBox1.BackColor = Color.White;
            reg.Find(richTextBox1.Text, caret);
            richTextBox1.SelectionLength = 0;
            richTextBox1.SelectionStart = caret;
            showReg(treeView2.Nodes, reg.ParserString);
            treeView2.ExpandAll();
            showReg(treeView3.Nodes, reg.PatternString);
            treeView3.ExpandAll();
        }
        private void showReg(TreeNodeCollection nodes, string str)
        {
            nodes.Clear();
            System.Text.RegularExpressions.Regex regex = new System.Text.RegularExpressions.Regex("(?m:^(?<pid>\\d+)\t(?<id>\\d+)\t(?<idt>\\d+)\t(?<idx>\\d+),(?<val>.+)\r\n)");
            System.Text.RegularExpressions.MatchCollection mtc = regex.Matches(str);
            foreach (System.Text.RegularExpressions.Match m in mtc)
            {
                TreeNode[] nl = nodes.Find(m.Groups["pid"].Value, true);
                if (nl.Count() > 0)
                    nl[0].Nodes.Add(m.Groups["id"].Value, m.Groups["val"].Value, m.Groups["success"].Value == "true" ? 1 : 0);
                else
                    nodes.Add(m.Groups["id"].Value, m.Groups["val"].Value, m.Groups["success"].Value == "true" ? 1 : 0);
            }
        }

        private void node2text(TreeNodeCollection nodes, ref StringBuilder str, int indent)
        {
            foreach (TreeNode node in nodes)
            {
                str.Append('\t', indent);
                str.Append(node.Text);
                str.Append("\r\n");
                node2text(node.Nodes, ref str, indent + 1);
            }
        }

        private void btnOpt_Click(object sender, EventArgs e)
        {
            TRegExpOption opt = reg.RegOption;
            if (btnOptM.Checked)
                opt |= TRegExpOption.optM;
            else
                opt &= TRegExpOption.optNotM;
            if (btnOptS.Checked)
                opt |= TRegExpOption.optS;
            else
                opt &= TRegExpOption.optNotS;
            if (btnOptI.Checked)
                opt |= TRegExpOption.optI;
            else
                opt &= TRegExpOption.optNotI;
            if (btnOptX.Checked)
                opt |= TRegExpOption.optX;
            else
                opt &= TRegExpOption.optNotX;
            reg.RegOption = opt;
        }

        private void toolStripButton3_Click(object sender, EventArgs e)
        {
            int caret = richTextBox1.SelectionStart;
            richTextBox1.SelectionStart = 0;
            richTextBox1.SelectionLength = richTextBox1.TextLength;
            richTextBox1.SelectionBackColor = Color.White;
            System.Drawing.Font font = richTextBox1.SelectionFont;
            richTextBox1.SelectionFont = font != null ? new Font(font.FontFamily, font.Size, FontStyle.Regular) : SystemFonts.DefaultFont;
            richTextBox1.SelectionLength = 0;
            richTextBox1.SelectionStart = caret;

            try
            {
                reg.Pattern = textBox1.Text;
            }
            catch
            {
                textBox1.BackColor = Color.Red;
                showReg(treeView2.Nodes, reg.ParserString);
                treeView2.ExpandAll();
                showReg(treeView3.Nodes, reg.PatternString);
                treeView3.ExpandAll();
                return;
            }

            textBox1.BackColor = Color.White;
            for (int l = 0; caret < richTextBox1.TextLength; ++caret)
            {
                l = reg.Match(richTextBox1.Text, caret);
                if (l > 0)
                {
                    richTextBox1.SelectionLength = 0;
                    richTextBox1.SelectionStart = caret;
                    richTextBox1.SelectionLength = l;
                    richTextBox1.SelectionBackColor = Color.LightGreen;
                    richTextBox1.SelectionLength = 0;
                    richTextBox1.SelectionStart = l + caret;
                    break;
                }
            }
            showReg(treeView2.Nodes, reg.ParserString);
            treeView2.ExpandAll();
            showReg(treeView3.Nodes, reg.PatternString);
            treeView3.ExpandAll();

        }

        private void toolStripButton5_Click(object sender, EventArgs e)
        {
            try
            {
                reg.Pattern = textBox1.Text;
            }
            catch
            {
                textBox1.BackColor = Color.Red;
                showReg(treeView2.Nodes, reg.ParserString);
                treeView2.ExpandAll();
                showReg(treeView3.Nodes, reg.PatternString);
                treeView3.ExpandAll();
                return;
            }
            textBox1.BackColor = Color.White;
            showReg(treeView2.Nodes, reg.ParserString);
            treeView2.ExpandAll();
            showReg(treeView3.Nodes, reg.PatternString);
            treeView3.ExpandAll();
        }

        [DllImport(@"Oleaut32.dll")]
        public static extern IntPtr SysAllocStringByteLen([MarshalAs(UnmanagedType.LPStr)] string psz, UInt32 len);

        [DllImport(@"Oleaut32.dll")]
        public static extern UInt32 SysFreeString(IntPtr bstr);

        private void toolStripButton4_Click(object sender, EventArgs e)
        {
            if (openFileDialog1.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                IF_MDLLib.IFRegExp REG = new IFRegExp();


                //{
                //    System.IO.StreamReader sr = new System.IO.StreamReader(openFileDialog1.FileName, Encoding.UTF8);
                //    richTextBox1.Text = REG.rtf_text(sr.ReadToEnd(), 2);
                //}

                {
                    System.IO.FileStream stream = System.IO.File.OpenRead(openFileDialog1.FileName);
                    byte[] buff = new byte[stream.Length];
                    stream.Read(buff, 0, (int)stream.Length);

                    IntPtr pBSTR = SysAllocStringByteLen(null, (UInt32)(buff.Length));
                    Marshal.Copy(buff, 0, pBSTR, buff.Length);

                    richTextBox1.Text = REG.rtf_text(Marshal.PtrToStringBSTR(pBSTR), 1);

                    Marshal.FreeBSTR(pBSTR);
                    pBSTR = IntPtr.Zero;
                }

                System.Windows.Forms.MessageBox.Show("OK");
            }
        }

        private void toolStripButton6_Click(object sender, EventArgs e)
        {
            string str = @"{\rtf1\ansi\ansicpg932\deff0\nouicompat\deflang1033\deflangfe1041{\fonttbl{\f0\fnil\fcharset128 \'82\'6c\'82\'72 \'96\'be\'92\'a9;}}
{\*\generator Riched20 6.3.9600}\viewkind4\uc1 
\pard\sa200\sl276\slmult1\f0\fs22\lang17 dfadf\'82\'6c\'82\'72 \'96\'be\'92\'a9v\par
}
 ";
            IF_MDLLib.IFRegExp REG = new IFRegExp();
            richTextBox1.Text = REG.rtf_text(str, 2);
        }
    }
}
