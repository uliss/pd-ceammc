use proc_macro::TokenStream;
use syn::{parse_macro_input, DeriveInput};
use quote::quote;

// extern crate proc_macro;
#[proc_macro_derive(PdError)]
pub fn impl_make_error_macro(input: TokenStream) -> TokenStream {
    let input = parse_macro_input!(input as DeriveInput);
    let name = input.ident;
    let gen = quote!{
        impl MakePdError<#name> for #name {
            fn pd_err(msg: CString) -> #name {
                #name::Error(msg)
            }
        }
    };

    gen.into()
}
