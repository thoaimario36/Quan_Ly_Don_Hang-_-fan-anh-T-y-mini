#include <iostream>
#include <string>
#include <iomanip>
#include <windows.h>

using namespace std;

//  CÁC HẰNG SỐ MÀU SẮC (Dùng cho giao diện Console )
#define MAU_TRANG       7
#define MAU_VANG        14
#define MAU_DO          12
#define MAU_XANH_LA     10
#define MAU_XANH_DUONG  11
#define MAU_TIM         13

void doiMau(int mau) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), mau);
}

//  LỚP NGÀY THÁNG
class NgayThang {
private:
    int ngay;
    int thang;
    int nam;

public:
    NgayThang() { ngay = 1; thang = 1; nam = 2024; }
    NgayThang(int ng, int th, int n) { ngay = ng; thang = th; nam = n; }
    NgayThang(const NgayThang &other) { ngay = other.ngay; thang = other.thang; nam = other.nam; }

    int getNgay() { return ngay; }
    void setNgay(int ng) { ngay = ng; }
    int getThang() { return thang; }
    void setThang(int th) { thang = th; }
    int getNam() { return nam; }
    void setNam(int n) { nam = n; }

    // Nhập ngày tháng thủ công từng biến
    void Nhap() {
        cout << "  Nhap ngay: "; cin >> ngay;
        cout << "  Nhap thang: "; cin >> thang;
        cout << "  Nhap nam: "; cin >> nam;
    }

    // Hàm trả về chuỗi dd/mm/yyyy để dễ dàng căn lề setw()
    string toString() {
        string s = "";
        if (ngay < 10) s += "0"; s += to_string(ngay) + "/";
        if (thang < 10) s += "0"; s += to_string(thang) + "/";
        s += to_string(nam);
        return s;
    }
};

//  LỚP CƠ SỞ: ĐƠN HÀNG (Lớp đa hình)
class DonHang {
protected: 
    string maDonHang;
    NgayThang ngayDatHang;
    int goiVanChuyen; // 1: Cơ bản, 2: Nhanh, 3: Hỏa tốc

public:
    DonHang() { maDonHang = "DH00"; goiVanChuyen = 1; }
    DonHang(string ma, NgayThang ngay, int goi) { maDonHang = ma; ngayDatHang = ngay; goiVanChuyen = goi; }
    virtual ~DonHang() {} // Hàm hủy ảo để xóa mảng con trỏ không bị rò rỉ RAM

    string getMaDonHang() { return maDonHang; }
    void setMaDonHang(string ma) { maDonHang = ma; }
    NgayThang getNgayDatHang() { return ngayDatHang; }
    void setNgayDatHang(NgayThang ngay) { ngayDatHang = ngay; }
    int getGoiVanChuyen() { return goiVanChuyen; }
    void setGoiVanChuyen(int goi) { goiVanChuyen = goi; }

    // Hàm thuần ảo 
    virtual double TinhTongTien() = 0;
    virtual string layTenLoaiDon() = 0; 

    // Phương thức ảo
    virtual void Nhap() {
        cout << "  Nhap ma don hang: ";
        cin.ignore();
        getline(cin, maDonHang);
        
        // Kiểm tra độ dài bằng.length()
        while(maDonHang.length() == 0) {
            cout << "  [!] Ma khong duoc rong. Nhap lai: ";
            getline(cin, maDonHang);
        }
        
        // Gọi hàm nhập của lớp NgayThang
        ngayDatHang.Nhap(); 
        
        cout << "  Chon goi van chuyen (1-Co ban, 2-Nhanh, 3-Hoa toc): ";
        cin >> goiVanChuyen;
    }

    virtual void InThongTin() {
        string tenGoi = "Co ban";
        if (goiVanChuyen == 2) tenGoi = "Nhanh";
        else if (goiVanChuyen == 3) tenGoi = "Hoa toc";

        // Dùng toString() của Ngày Tháng để chuỗi không bị gãy khi căn lề
        cout << left << setw(12) << maDonHang 
             << setw(15) << ngayDatHang.toString() 
             << setw(15) << layTenLoaiDon()
             << setw(15) << tenGoi;
    }
};

//  LỚP KẾ THỪA 1: ĐƠN HÀNG TRONG NƯỚC
class DonHangTrongNuoc : public DonHang {
private:
    double phiVanChuyen;

public:
    DonHangTrongNuoc() : DonHang() { phiVanChuyen = 0; }
    DonHangTrongNuoc(string ma, NgayThang ngay, int goi, double phi) : DonHang(ma, ngay, goi) { phiVanChuyen = phi; }

    double getPhiVanChuyen() { return phiVanChuyen; }
    void setPhiVanChuyen(double phi) { phiVanChuyen = phi; }

    string layTenLoaiDon() override { return "Trong Nuoc"; }

    void Nhap() override {
        doiMau(MAU_XANH_DUONG);
        cout << "\n--- NHAP DON HANG TRONG NUOC ---\n";
        doiMau(MAU_TRANG);
        
        DonHang::Nhap(); // Gọi lại hàm nhập của cha
        cout << "  Nhap phi van chuyen goc (VND): ";
        cin >> phiVanChuyen;
    }

    double TinhTongTien() override {
        double heSo = 1.0;
        if (goiVanChuyen == 2) heSo = 1.5;
        if (goiVanChuyen == 3) heSo = 2.0;
        return phiVanChuyen * heSo;
    }

    void InThongTin() override {
        DonHang::InThongTin();
        cout << setw(15) << fixed << setprecision(0) << phiVanChuyen 
             << setw(20) << TinhTongTien() << endl;
    }
};

//  LỚP KẾ THỪA 2: ĐƠN HÀNG QUỐC TẾ
class DonHangQuocTe : public DonHang {
private:
    double phiHaiQuan; // Tính bằng USD

public:
    DonHangQuocTe() : DonHang() { phiHaiQuan = 0; }
    DonHangQuocTe(string ma, NgayThang ngay, int goi, double phiHQ) : DonHang(ma, ngay, goi) { phiHaiQuan = phiHQ; }

    double getPhiHaiQuan() { return phiHaiQuan; }
    void setPhiHaiQuan(double phiHQ) { phiHaiQuan = phiHQ; }

    string layTenLoaiDon() override { return "Quoc Te"; }

    void Nhap() override {
        doiMau(MAU_XANH_DUONG);
        cout << "\n--- NHAP DON HANG QUOC TE ---\n";
        doiMau(MAU_TRANG);
        
        DonHang::Nhap();
        cout << "  Nhap phi Hai Quan (USD): ";
        cin >> phiHaiQuan;
    }

    double TinhTongTien() override {
        double phiVND = phiHaiQuan * 25000; 
        double tongTien = phiVND;
        if (goiVanChuyen == 2) tongTien = (phiVND * 1.5) + 500000; // Phụ phí gói
        if (goiVanChuyen == 3) tongTien = (phiVND * 2.0) + 1500000; 
        return tongTien;
    }

    void InThongTin() override {
        DonHang::InThongTin();
        cout << setw(15) << fixed << setprecision(2) << phiHaiQuan 
             << setw(20) << setprecision(0) << TinhTongTien() << endl;
    }
};

//  LỚP QUẢN LÝ ĐƠN HÀNG
class QuanLyDonHang {
private:
    DonHang* danhSach[100]; // Mảng con trỏ chứa tối đa 100 đơn
    int soLuong;
public:
    QuanLyDonHang() {
        soLuong = 0; // Khởi tạo số lượng ban đầu bằng 0
    }

    void ThemDonHang(int loai) {
        if (soLuong >= 100) {
            doiMau(MAU_DO);
            cout << "\n[!] Bo nho da day, khong the them!\n";
            doiMau(MAU_TRANG);
            return;
        }

        DonHang* dh = NULL;
        if (loai == 1) dh = new DonHangTrongNuoc();
        else if (loai == 2) dh = new DonHangQuocTe();
        
        if (dh != NULL) {
            dh->Nhap();
            danhSach[soLuong] = dh;
            soLuong++; // Tăng biến đếm lên
            
            doiMau(MAU_XANH_LA);
            cout << "=> [OK] Them don hang thanh cong!\n";
            doiMau(MAU_TRANG);
        }
    }

    void XuatDanhSach() {
        if (soLuong == 0) {
            doiMau(MAU_DO);
            cout << "\n[!] Danh sach dang rong!\n";
            doiMau(MAU_TRANG);
            return;
        }
        
        doiMau(MAU_TIM);
        cout << "\n------------------------------------------------------------------------------------------\n";
        cout << left << setw(12) << "MA DH" 
             << setw(15) << "NGAY DAT" 
             << setw(15) << "LOAI DON" 
             << setw(15) << "GOI VC" 
             << setw(15) << "PHI(GOC/USD)" 
             << setw(20) << "TONG TIEN (VND)" << endl;
        cout << "------------------------------------------------------------------------------------------\n";
        doiMau(MAU_TRANG);
        
        for (int i = 0; i < soLuong; i++) {
            danhSach[i]->InThongTin();
        }
        
        doiMau(MAU_TIM);
        cout << "------------------------------------------------------------------------------------------\n";
        doiMau(MAU_TRANG);
    }

    ~QuanLyDonHang() {
        // Dọn dẹp RAM bằng vòng lặp for cơ bản
        for (int i = 0; i < soLuong; i++) {
            delete danhSach[i];
        }
    }
};

//  HÀM MAIN
int main() {
    QuanLyDonHang ql;
    int luaChon;

    do {
        doiMau(MAU_VANG);
        cout << "\n_____________________HE THONG QUAN LY DON HANG_____________________\n";
        cout<< "\n"; 
        cout << "  1. Them don hang Trong Nuoc\n";
        cout << "  2. Them don hang Quoc Te\n";
        cout << "  3. Xem danh sach don hang\n";
        cout << "  0. Thoat chuong trinh\n";
		        
        doiMau(MAU_DO);
        cout << ">> Nhap lua chon: ";
        doiMau(MAU_TRANG);
        cin >> luaChon;

        switch (luaChon) {
            case 1: ql.ThemDonHang(1); break;
            case 2: ql.ThemDonHang(2); break;
            case 3: ql.XuatDanhSach(); break;
            case 0: 
                doiMau(MAU_XANH_LA);
                cout << "\nDang thoat chuong trinh. Tam biet!\n";
                doiMau(MAU_TRANG);
                break;
            default:
                doiMau(MAU_DO);
                cout << "\n[!] Lua chon sai. Vui long nhap lai!\n";
                doiMau(MAU_TRANG);
                break;
        }
    } while (luaChon != 0);

    return 0;
}
