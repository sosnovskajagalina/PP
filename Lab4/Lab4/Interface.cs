using System.Windows.Forms;

namespace Lab4
{
    public partial class Interface : InputForm
    {
        private readonly Controller _viewController = new Controller();
        
        public Interface()
        {
            CreateInterface();
            InputFild();
            Button();
        }
        private void CreateInterface()
        {
            this.ClientSize = new System.Drawing.Size(500, 500);
            this.Text = "Interface";
            this.ResumeLayout(false);
        }

        private void Button()
        {
            var okButton = new Button {Text = @"Go"};
            okButton.MouseClick += _viewController.GoButton;
            Panel.Controls.Add(okButton);
        }
        
        private void InputFild()
        {
            SaveMain.Input = "Price";
            SaveMain.Output = "Convert";
            SaveMain.CountIteration = 18;

            var InputBox = AddTextBox("Input file name: ", "Price");
            var OutputBox = AddTextBox("Output file name: ", "Convert");
            var countIterationBox = AddTextBox("Count iteration: ", "18");
            
            InputBox.KeyUp += _viewController.OnInput;
            OutputBox.KeyUp += _viewController.OnOutput;
            countIterationBox.KeyUp += _viewController.OnChangeCountIteration;
        }
    }
}