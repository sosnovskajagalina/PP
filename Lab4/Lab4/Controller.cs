using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;
using Lab4.Services;
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;

namespace Lab4
{
    public class Controller
    {
        private readonly Transport _transport = new Transport();
        
        private async Task GetAsynchronous()
        {
            var acceptCurrenciesTask = GetAcceptCurrencies();
            var dataTask = _transport.GetCurrenciesData();
            
            await Task.WhenAll(acceptCurrenciesTask, dataTask);

            var acceptCurrencies = await acceptCurrenciesTask;
            var data = await dataTask;
            var isFilter = acceptCurrencies != null;

            using (var writer = new StreamWriter("../../Convert.txt"))
            {
                IDictionary<string, JToken> currencies = data.Valute;
                foreach (var item in currencies)
                {
                    if (isFilter && !acceptCurrencies.Contains(item.Key)) continue;
                    writer.WriteLineAsync($"{item.Value["Nominal"]} {item.Key} Price {item.Value["Value"]} RUB");
                }
            }
        }

        private void GetSynchronous()
        {
            var acceptCurrencies = GetAcceptCurrencies().Result;
            var data = _transport.GetCurrenciesData().Result;
            var isFilter = acceptCurrencies != null;

            using (var writer = new StreamWriter("../../Convert.txt"))
            {
                IDictionary<string, JToken> currencies = data.Valute;
                foreach (var item in currencies)
                {
                    if (isFilter && !acceptCurrencies.Contains(item.Key)) continue;
                    writer.WriteLine($"{item.Value["Nominal"]} {item.Key} Price {item.Value["Value"]} RUB");
                }
            }
        }

        public async void GoButton(object sender, EventArgs e)
        {
            var elapsedMs = new List<long>();
            for (var i = 0; i < SaveMain.CountIteration; i++)
            {
                var watch = Stopwatch.StartNew();

                var newThread = new Thread(GetSynchronous);
                newThread.Start();
                newThread.Join();

                watch.Stop();
                elapsedMs.Add(watch.ElapsedMilliseconds);
            }
            MessageBox.Show("Sync: \n" + elapsedMs.Sum(time => time) / SaveMain.CountIteration);

            elapsedMs = new List<long>();
            for (var i = 0; i < SaveMain.CountIteration; i++)
            {
                var watch = Stopwatch.StartNew();

                await GetAsynchronous();

                watch.Stop();
                elapsedMs.Add(watch.ElapsedMilliseconds);
            }
            MessageBox.Show("Async: \n" + elapsedMs.Sum(time => time) / SaveMain.CountIteration);
        }

        private async Task<List<string>> GetAcceptCurrencies()
        {
            var pathAcceptCurrencies = $"../../Price.json";
            if (!File.Exists(pathAcceptCurrencies)) return null;
            using (var reader = new StreamReader(pathAcceptCurrencies))
            {
                var file = await reader.ReadToEndAsync();
                return JsonConvert.DeserializeObject<List<string>>(file);
            }
        }
        
        public void OnChangeCountIteration(object sender, KeyEventArgs e)
        {
            var value = (TextBox) sender;
            if (value.TextLength == 0)
            {
                SaveMain.CountIteration = 0;
                return;
            }
            if (int.TryParse(value.Text, out var count)) SaveMain.CountIteration = count;
        }
        
        public void OnInput(object sender, KeyEventArgs e)
        {
            var value = (TextBox) sender;
            SaveMain.Input = value.Text;
        }
        
        public void OnOutput(object sender, KeyEventArgs e)
        {
            var value = (TextBox) sender;
            SaveMain.Output = value.Text;
        }
        
    }
}