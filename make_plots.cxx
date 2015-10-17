int make_plots(const char * mat = "Au", int length=15, int runnumber=5, double OD = 15)
{

   int OD_print = OD;

   TFile * f = new TFile(Form("bubble_%s_%dcm_%dmm_%d.root",mat,length,OD_print,runnumber),"UPDATE");
   f->cd();
   f->ls();

   TLatex lt;
   lt.SetNDC(true);

   TCanvas * c = new TCanvas();
   c->cd(0);
   gPad->SetLogy(true);
   TH1D * h1 = (TH1D*)f->FindObjectAny("/FakeSD1/forw0");
   TH1D * hdenom = h1;
   if(h1){
      h1->Draw("hist");
      h1->GetXaxis()->SetTitle("E (MeV)");
   }
   h1 = (TH1D*)f->FindObjectAny("/FakeSD2/forw0");
   if(h1){
      h1->Draw("hist,same");
   }
   TH1D * hnum = h1;

   c->SaveAs(Form("Energy_in_out_%s_%dcm_%dmm_%d.pdf",mat,length,OD_print,runnumber));

   // -------------------------

   c = new TCanvas();

   hnum->Sumw2();
   hdenom->Sumw2();

   hnum->Divide(hdenom);
   hnum->SetTitle("After/Before");
   hnum->Draw("hist,E1");
   hnum->GetXaxis()->SetTitle("E (MeV)");
   hnum->GetYaxis()->SetRangeUser(0.0,0.20);

   c->SaveAs(Form("Ratio_in_out_%s_%dcm_%dmm_%d.pdf",mat,length,OD_print,runnumber));

   // -------------------------
   TCanvas * c2 = new TCanvas();
   c2->Divide(2,2);

   c2->cd(1);
   TH2D * h2 = (TH2D*)f->FindObjectAny("/FakeSD1/fhXY0_all");
   if(h2){
      h2->Draw("colz");
      h2->SetTitle("Before collimator");
      h2->GetXaxis()->SetTitle("x (cm)");
      h2->GetYaxis()->SetTitle("y (cm)");
      h2->Draw("colz");
   }
   c2->cd(2);
   h2 = (TH2D*)f->FindObjectAny("/FakeSD2/fhXY0_all");
   if(h2){
      h2->Draw("colz");
      h2->SetTitle("After collimator");
      h2->GetXaxis()->SetTitle("x (cm)");
      h2->GetYaxis()->SetTitle("y (cm)");
      h2->Draw("colz");
   }
   c2->cd(3);
   h2 = (TH2D*)f->FindObjectAny("/FakeSD1/fhXY2_all");
   if(h2){
      h2->Draw("colz");
      h2->SetTitle("Before collimator");
      h2->GetXaxis()->SetTitle("x (cm)");
      h2->GetYaxis()->SetTitle("y (cm)");
      h2->Draw("colz");
   }
   c2->cd(4);
   h2 = (TH2D*)f->FindObjectAny("/FakeSD2/fhXY2_all");
   if(h2){
      h2->Draw("colz");
      h2->SetTitle("After collimator");
      h2->GetXaxis()->SetTitle("x (cm)");
      h2->GetYaxis()->SetTitle("y (cm)");
      h2->Draw("colz");
   }

   c2->cd(1);
   lt.DrawLatex(0.0,0.0,Form("L = %dcm, %s",length,mat));

   c2->SaveAs(Form("XY_before_after_%s_%dcm_%dmm_%d.pdf",mat,length,OD_print,runnumber));

   // -------------------------
   //
   c2 = new TCanvas();
   c2->Divide(2,1);

   c2->cd(1);
   h2 = (TH2D*)f->FindObjectAny("/FakeSD1/fhXvsE_all");
   if(h2){
      h2->Draw("colz");
      h2->SetTitle("Before collimator");
      h2->GetXaxis()->SetTitle("x (cm)");
      h2->GetYaxis()->SetTitle("y (cm)");
      h2->Draw("colz");
   }
   c2->cd(2);
   h2 = (TH2D*)f->FindObjectAny("/FakeSD2/fhXvsE_all");
   if(h2){
      h2->Draw("colz");
      h2->SetTitle("Before collimator");
      h2->GetXaxis()->SetTitle("x (cm)");
      h2->GetYaxis()->SetTitle("y (cm)");
      h2->Draw("colz");
   }
   c2->SaveAs(Form("XvsE_before_after_%s_%dcm_%dmm_%d.pdf",mat,length,OD_print,runnumber));
   return 0;

}
