using System;
using System.Collections.Generic;
using System.Drawing;
using System.Windows.Forms;
using System.Runtime.InteropServices;

namespace mugenhelper
{
	public partial class MainForm : Form
	{
		[DllImport("kernel32.dll")]
		private static extern int GetPrivateProfileInt(string section, string key, int def, string fileName);

		[DllImport("kernel32.dll")]
		private static extern int GetPrivateProfileString(string section, string key, string def, System.Text.StringBuilder returnValue, int size, string fileName);

		[DllImport("kernel32.dll")]
		private static extern int GetPrivateProfileSection(string section, IntPtr returnValue, int size, string fileName);

		[DllImport("kernel32.dll")]
		private static extern long WritePrivateProfileString(string section, string key, string value, string fileName);
		public MainForm()
		{
			InitializeComponent();
		}

		void Button1Click(object sender, EventArgs e)
		{
			textBox1.Clear();
			
			textBox1.AppendText("[Characters]");
			textBox1.AppendText(Environment.NewLine);

			System.IO.DirectoryInfo dirChars = new System.IO.DirectoryInfo(Application.StartupPath + "\\chars\\");

			System.IO.DirectoryInfo[] foldersChars = dirChars.GetDirectories("*.*");

			foreach (System.IO.DirectoryInfo di in foldersChars)
			{
				string tempPath = di.FullName + "_temp";
				string newPath = di.FullName.ToLower();
				System.IO.Directory.Move(di.FullName, tempPath);
				System.IO.Directory.Move(tempPath, newPath);
				
				textBox1.AppendText(di.Name.ToLower());
				textBox1.AppendText(Environment.NewLine);
			}

			textBox1.AppendText(Environment.NewLine);

			textBox1.AppendText("[ExtraStages]");
			textBox1.AppendText(Environment.NewLine);

			System.IO.DirectoryInfo dirStages = new System.IO.DirectoryInfo(Application.StartupPath + "\\stages\\");

			System.IO.FileInfo[] filesStagesDef = dirStages.GetFiles("*.def");
			System.IO.FileInfo[] filesStagesSff = dirStages.GetFiles("*.sff");

			foreach (System.IO.FileInfo fi in filesStagesDef)
			{
				fi.MoveTo(fi.FullName.ToLower());

				textBox1.AppendText("stages/" + fi.Name.ToLower());
				textBox1.AppendText(Environment.NewLine);
			}

			foreach (System.IO.FileInfo fi in filesStagesSff)
				fi.MoveTo(fi.FullName.ToLower());

			textBox1.AppendText(Environment.NewLine);

			textBox1.AppendText("[Options]");
			textBox1.AppendText(Environment.NewLine);
			textBox1.AppendText("arcade.maxmatches = 3,2,1,0,0,0,0,0,0,0");
			textBox1.AppendText(Environment.NewLine);
			textBox1.AppendText("team.maxmatches = 3,2,1,0,0,0,0,0,0,0");
		}

		void Button2Click(object sender, EventArgs e)
		{
			checkedListBox1.Items.Clear();

			System.IO.DirectoryInfo dirStages = new System.IO.DirectoryInfo(Application.StartupPath + "\\stages\\");

			System.IO.FileInfo[] filesStagesDef = dirStages.GetFiles("*.def");

			foreach (System.IO.FileInfo fi in filesStagesDef)
			{
				checkedListBox1.Items.Add(fi.FullName.ToLower());

				for(int i = 0;i < checkedListBox1.Items.Count; i++)
				{
					string stage = checkedListBox1.Items[i].ToString();

					if (stage.Length == 0)
						continue;

					int hires = GetPrivateProfileInt("stageinfo", "hires", 0, stage);
					if (hires == 1)
						checkedListBox1.SetItemChecked(i, true);
					else
						checkedListBox1.SetItemChecked(i, false);
				}
			}
		}

		void Button3Click(object sender, EventArgs e)
		{
			if (checkedListBox1.Items.Count == 0)
				return;

			if (MessageBox.Show("Are you sure you want to save these hires values?", "Save hires", MessageBoxButtons.YesNo, MessageBoxIcon.Question) == DialogResult.No)
				return;

			for(int i = 0;i < checkedListBox1.Items.Count; i++)
			{
				string stage = checkedListBox1.Items[i].ToString();

				if (stage.Length == 0)
					continue;

				if (checkedListBox1.GetItemChecked(i))
					WritePrivateProfileString("stageinfo", "hires", "1", stage);
				else
					WritePrivateProfileString("stageinfo", "hires", "0", stage);
			}		
		}

		void Button4Click(object sender, EventArgs e)
		{
			if (textBox1.TextLength == 0)
				return;

			if (MessageBox.Show("Are you sure you want to overwrite select.def?", "Write to select.def", MessageBoxButtons.YesNo, MessageBoxIcon.Question) == DialogResult.No)
				return;

			string selectDef = Application.StartupPath + "\\data\\select.def";

			System.IO.File.WriteAllLines(selectDef, textBox1.Lines);
		}

		void Button5Click(object sender, EventArgs e)
		{
			if (checkedListBox1.Items.Count == 0)
				return;

			for(int i = 0;i < checkedListBox1.Items.Count; i++)
			{
				string stage = checkedListBox1.Items[i].ToString();

				if (stage.Length == 0)
					continue;
	
				int hires = GetPrivateProfileInt("stageinfo", "hires", 0, stage);
				if (hires == 1)
						checkedListBox1.SetItemChecked(i, true);
				else
						checkedListBox1.SetItemChecked(i, false);
			}
		}

		void Button6Click(object sender, EventArgs e)
		{
			listView1.Items.Clear();

			ListViewItem lvi;

			string[] columnHeaders = new string[2];

			System.IO.DirectoryInfo dirStages = new System.IO.DirectoryInfo(Application.StartupPath + "\\stages\\");
			System.IO.FileInfo[] filesStagesDef = dirStages.GetFiles("*.def");

			foreach (System.IO.FileInfo fi in filesStagesDef)
			{
				string stage = fi.FullName;

				System.Text.StringBuilder temp = new System.Text.StringBuilder(255);
				int i = GetPrivateProfileString("music", "bgmusic", "", temp, 255, stage);
				string music = temp.ToString();

				music = music.Replace("\\", "/");

				columnHeaders[0] = stage;
				columnHeaders[1] = music;
				lvi = new ListViewItem(columnHeaders);
				listView1.Items.Add(lvi);
			}
		}

		void Button7Click(object sender, EventArgs e)
		{
			if (listView1.Items.Count == 0)
				return;

			if (listView1.SelectedItems.Count == 0)
				return;

			ListViewItem lvi = listView1.SelectedItems[0];

			int i = listView1.Items.IndexOf(lvi);

			OpenFileDialog ofn = new OpenFileDialog();
			ofn.InitialDirectory = Application.StartupPath + "\\sound\\";
			ofn.Filter = "All Files (*.*)|*.*";
			ofn.Title = "Select stage";

			if (ofn.ShowDialog() == DialogResult.Cancel)
				return;

			string music = "sound/" + System.IO.Path.GetFileName(ofn.FileName);

			if (music.Length == 0)
				return;

			listView1.Items[i].SubItems[1].Text = music;
		}

		void Button8Click(object sender, EventArgs e)
		{
			if (listView1.Items.Count == 0)
				return;

			if (MessageBox.Show("Are you sure you want to save these music values?", "Save music", MessageBoxButtons.YesNo, MessageBoxIcon.Question) == DialogResult.No)
				return;

			for(int i = 0;i < listView1.Items.Count; i++)
			{
				string stage = listView1.Items[i].SubItems[0].Text;
				string music = listView1.Items[i].SubItems[1].Text;

				if (stage.Length == 0)
					continue;

				WritePrivateProfileString("music", "bgmusic", music, stage);
				WritePrivateProfileString("music", "bgvolume", "255", stage);
			}
		}

		void Button9Click(object sender, EventArgs e)
		{
			if (listView1.Items.Count == 0)
				return;

			System.IO.DirectoryInfo dirStages = new System.IO.DirectoryInfo(Application.StartupPath + "\\stages\\");
			System.IO.FileInfo[] filesStagesDef = dirStages.GetFiles("*.def");

			foreach (System.IO.FileInfo fi in filesStagesDef)
			{
				string stage = fi.FullName;

				System.Text.StringBuilder temp = new System.Text.StringBuilder(255);
				int i = GetPrivateProfileString("music", "bgmusic", "", temp, 255, stage);
				string music = temp.ToString();

				ListViewItem lvi = listView1.FindItemWithText(stage, false, 0, false);
				lvi.SubItems[0].Text = stage;
				lvi.SubItems[1].Text = music;
			}
		}

		void Button10Click(object sender, EventArgs e)
		{
			if (listView1.Items.Count == 0)
				return;

			if (listView1.SelectedItems.Count == 0)
				return;

			ListViewItem lvi = listView1.SelectedItems[0];

			int i = listView1.Items.IndexOf(lvi);

			listView1.Items[i].SubItems[1].Text = "";
		}

		void Button11Click(object sender, EventArgs e)
		{
			if (listView1.Items.Count == 0)
				return;

			if (listView1.SelectedItems.Count == 0)
				return;

			ListViewItem lvi = listView1.SelectedItems[0];

			int i = listView1.Items.IndexOf(lvi);

			string music = listView1.Items[i].SubItems[1].Text;

			if (music.Length == 0)
				return;

			string remove = "sound/";
			music = music.Remove(0, remove.Length);

			string fileName = Application.StartupPath + "\\sound\\" + music;

			if (!System.IO.File.Exists(fileName))
				return;

			System.Diagnostics.Process proc = new System.Diagnostics.Process();
			proc.EnableRaisingEvents = false;
			proc.StartInfo.FileName = "mplayerc"; // mplayer2, mplayerc, winamp
			proc.StartInfo.Arguments = fileName;
			proc.Start();
		}

		void MainFormLoad(object sender, EventArgs e)
		{
			if (!System.IO.File.Exists(Application.StartupPath + "\\winmugen.exe"))
			{
				MessageBox.Show("winmugen.exe not found!", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
				Application.Exit();
			}
		}

		void Button12Click(object sender, EventArgs e)
		{
			textBox1.Text = System.IO.File.ReadAllText(Application.StartupPath + "\\data\\select.def");
		}

		void Button13Click(object sender, EventArgs e)
		{
			textBox2.Clear();

			System.IO.DirectoryInfo dirChars = new System.IO.DirectoryInfo(Application.StartupPath + "\\chars\\");

			System.IO.DirectoryInfo[] foldersChars = dirChars.GetDirectories("*.*");

			foreach (System.IO.DirectoryInfo di in foldersChars)
			{
				textBox2.AppendText(di.Name.ToLower());
				textBox2.AppendText(Environment.NewLine);
			}
		}

		void Button14Click(object sender, EventArgs e)
		{
			textBox2.Clear();

			System.IO.DirectoryInfo dirStages = new System.IO.DirectoryInfo(Application.StartupPath + "\\stages\\");

			System.IO.FileInfo[] filesStagesDef = dirStages.GetFiles("*.def");

			foreach (System.IO.FileInfo fi in filesStagesDef)
			{
				fi.MoveTo(fi.FullName.ToLower());

				textBox2.AppendText("stages/" + fi.Name.ToLower());
				textBox2.AppendText(Environment.NewLine);
			}
		}
	}
}
