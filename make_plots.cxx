int make_plots(const char * mat = "Cu", int length=10, int runnumber=10)
{

   TFile * f = new TFile(Form("bubble_%s_%dcm_%d.root",mat,length,runnumber),"UPDATE");
   f->cd();

   TLatex lt;
   lt.SetNDC(true);

   TCanvas * c = new TCanvas();
   c->cd(0);
   gPad->SetLogy(true);
   TH1D * h1 = (TH1D*)f->FindObjectAny("/FakeSD1/forw0");
   if(h1){
      h1->Draw("hist");
      h1->GetXaxis()->SetTitle("E (MeV)");
   }
   h1 = (TH1D*)f->FindObjectAny("/FakeSD2/forw0");
   if(h1){
      h1->Draw("hist,same");
   }

   c->SaveAs(Form("Energy_in_out_%s_%dcm_%d.pdf",mat,length,runnumber));

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

   c2->SaveAs(Form("XY_before_after_%s_%dcm_%d.pdf",mat,length,runnumber));

   return 0;

}
