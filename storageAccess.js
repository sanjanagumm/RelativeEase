function doThingsWithCookies() {
    document.cookie = "foo=bar"; 
  }
  
  async function handleCookieAccess() {
    if (!document.hasStorageAccess) {
      doThingsWithCookies();
    } else {
      const hasAccess = await document.hasStorageAccess();
      if (hasAccess) {
        doThingsWithCookies();
      } else {
        try {
          const permission = await navigator.permissions.query({
            name: "storage-access",
          });
  
          if (permission.state === "granted") {
            await document.requestStorageAccess();
            doThingsWithCookies();
          } else if (permission.state === "prompt") {
            btn.addEventListener("click", async () => {
              try {
                await document.requestStorageAccess();
                doThingsWithCookies();
              } catch (err) {
                console.error(`Error obtaining storage access: ${err}.
                              Please sign in.`);
              }
            });
          } else if (permission.state === "denied") {
          }
        } catch (error) {
          console.log(Could not access permission state. Error: ${error});
          doThingsWithCookies(); 
        }
      }
    }
}