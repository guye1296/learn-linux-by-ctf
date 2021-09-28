module.exports = {
  siteMetadata: {
    title: "Linux by CTF",
  },
  plugins: [
    {
      resolve: "gatsby-source-filesystem",
      options: {
        name: `challenges`,
        path: `${__dirname}/challenges`
        // TODO: include only .md files
      }
    },
    "gatsby-transformer-remark",
    "@arnau/gatsby-transformer-toml",
  ],
};
