use proc_macro::TokenStream;
use quote::quote;
use syn::{parse_macro_input, DeriveInput};

// extern crate proc_macro;
#[proc_macro_derive(PdError)]
pub fn impl_make_error_macro(input: TokenStream) -> TokenStream {
    let input = parse_macro_input!(input as DeriveInput);
    let name = input.ident;
    let gen = quote! {
        impl MakePdError<#name> for #name {
            fn pd_err(msg: CString) -> #name {
                #name::Error(msg)
            }
        }
    };

    gen.into()
}

// extern crate proc_macro;
#[proc_macro_derive(PdDebug)]
pub fn impl_make_debug_macro(input: TokenStream) -> TokenStream {
    let input = parse_macro_input!(input as DeriveInput);
    let name = input.ident;
    let gen = quote! {
        impl MakePdDebug<#name> for #name {
            fn pd_debug(msg: CString) -> #name {
                #name::Debug(msg)
            }
        }
    };

    gen.into()
}
